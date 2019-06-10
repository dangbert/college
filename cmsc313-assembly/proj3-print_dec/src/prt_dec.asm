; Dan Engbert CMSC 313 Spring 2017
;
; subroutine that takes a 32 bit number as a parameter and converts it to ascii
; and then prints it out

%define STDIN 0
%define STDOUT 1
%define SYSCALL_EXIT  1
%define SYSCALL_READ  3
%define SYSCALL_WRITE 4
%define BUFLEN 10                       ; parameter must be <= 10 decimal digits


        ; uninitialized data section (allocates memory)
        SECTION .bss
buf:    resb BUFLEN                     ; buffer for inputed string

        ; initialized data section
        SECTION .data

        ; code section
        SECTION .text
        global  prt_dec

; subroutine that prints out a decimal parameter passed on the stack
; parameter is 4 bytes in size (i.e. up to 10 decimal digits)
prt_dec: nop
        push    ebp
        mov     ebp, esp                 ; base pointer
        ; function params are now at ebp + 8

        ; push all the registers we need to use
        push    eax
        push    ebx
        push    ecx
        push    edx
        push    esi

        ; start at end of buffer b/c the parameter digits are read from right to left
        mov     ecx, buf + BUFLEN       ; location (in buffer) for the next char
        mov     esi, 0                  ; length of string in chars
        mov     eax, [ebp + 8]          ; parameter value (number) to be printed
        mov     ebx, 10                 ; value to divide by

        ; divide edx:eax by ebx
        ; quotient goes to eax
        ; remainder goes to edx
before: nop
        mov     edx, 0                  ; zero out
        inc     esi                     ; keep track of number of chars
        dec     ecx                     ; move back by 1 char in buffer
        div     ebx                     ; divide number by 10
        add     edx, 48                 ; 48 is ascii for '0'
        mov     [ecx], dl               ; write char to buffer
copied: nop

        cmp     eax, 0
        je      after                   ; stop when quotient is 0
        jmp     before

after:  nop
        ; calculate address of the start of message and store it in ecx
        mov     ecx, buf
        add     ecx, BUFLEN
        sub     ecx, esi


        mov     eax, SYSCALL_WRITE      ; write function
        mov     ebx, STDOUT             ; Arg1: file descriptor
        mov     edx, esi                ; Arg3: length of message
print:  nop
        int     080h                    ; ask kernel to write

        ; end of funtion
        ; restore register values
        pop     esi
        pop     edx
        pop     ecx
        pop     ebx
        pop     eax
        pop     ebp
        ret


        ; final exit
exit:   mov     eax, SYSCALL_EXIT       ; exit function
        mov     ebx, 0                  ; exit code, 0=normal
        int     080h                    ; ask kernel to take over
