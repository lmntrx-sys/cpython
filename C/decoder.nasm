bits 64

global decode_and_reverse
section .note.GNU-stack noalloc noexec nowrite progbits
section .text

decode_and_reverse:
    ;rdi buffer 64bit
    ;rsi length 
    ;dl key 8bit
    push rbx             ; Save rbx on the stack
    xor rcx, rcx         ; rcx = 0 (loop counter)

.xor_loop:
    cmp rcx, rsi
    jge .xor_done

    mov al, byte[rdi+rcx]
    xor al, dl 
    mov byte[rdi+rcx], al

    inc rcx
    jmp .xor_loop

.xor_done:

    mov rbx, rsi         ; rbx = length
    dec rbx              ; rbx = length - 1 (last index)
    xor rcx, rcx         ; rcx = 0 (start index)

.termination:
    cmp rcx, rbx
    jge .reverse_done

    ; perform swap
    mov al, byte[rdi+rcx]         ; load byte at start index
    mov r8b, byte[rdi+rbx]        ; load byte at end index
    mov byte[rdi+rcx], r8b        ; store end byte at start index
    mov byte[rdi+rbx], al         ; store start byte at end index

    inc rcx                      ; move to next start index
    dec rbx                      ; move to previous end index
    jmp .termination

.reverse_done:
    pop rbx               ; Restore rbx from the stack
    ret

    
