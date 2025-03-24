section .data
    num1 db 10      ; First number
    num2 db 5     ; Second number
    result db 0    ; To store the result

section .text
    global _start

add:
    ; Load the numbers into registers
    mov al, [num1] ; Load num1 into AL register
    add al, [num2] ; Add num2 to AL register
    add [result], al ; Store the result
    ret

_start:

    call add

    mov byte [add + 7], 42

    call add

    ; Exit the program
    mov eax, 60    ; syscall: exit
    mov edi, [result]   ; status: 0
    syscall
