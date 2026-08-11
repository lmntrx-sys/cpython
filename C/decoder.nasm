bits 64

global decode_and_revrse
section .text

decode_and_revrse:
    ; Function prologue
    push rbp
    mov rbp, rsp

    ; RDI = char *buffer, RSI = size_t length, RDX = char key
    mov r8, rdi          ; Save buffer pointer in r8 
    mov r9, rsi          ; Save length in r9
    mov r10, rdx         ; Save key in r10