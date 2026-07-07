from pwn import *
from ctypes import *

# calculate the value of eax 
def signed(x):
    return c_int(x).value

def calculate(op):
    ov = signed(op)
    edx = signed(0xffffbfb8)
    return (ov - 4 -edx) / 4 # Out we get eax

main_ret = calculate(0xffffc57c)
print("main_ret: ", main_ret)