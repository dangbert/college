; Dan Engbert CMSC 313 Spring 2017
;
; Applys ROT13 (shift cipher) to user input and outputs the result
; (non alphabetic characters are not modified)

%define STDIN 0
%define STDOUT 1
%define SYSCALL_EXIT  1
%define SYSCALL_READ  3
%define SYSCALL_WRITE 4
%define BUFLEN 256


        ; initialized data section
        SECTION .data
msg1:   db "Enter string: "             ; user prompt message
len1:   equ $-msg1                      ; length of first message

msg2:   db "Original: "                 ; original string label
len2:   equ $-msg2

msg3:   db "Convert:  "                 ; converted string label
len3:   equ $-msg3

msg4:   db 10, "Read error", 10         ; error message
len4:   equ $-msg4


        ; uninitialized data section (allocates memory)
        SECTION .bss
buf:    resb BUFLEN                     ; buffer for inputed string
res:    resb BUFLEN                     ; converted string (has same length)
rlen:   resb 4                          ; length of string(s)


        ; code section
        SECTION .text
        global  _start                  ; let loader see entry point
_start: nop                             ; entry point
        ; prompt user for input
        mov     eax, SYSCALL_WRITE      ; write function
        mov     ebx, STDOUT             ; Arg1: file descriptor
        mov     ecx, msg1               ; Arg2: addr of message
        mov     edx, len1               ; Arg3: length of message
        int     080h                    ; ask kernel to write

        ; read user input
        mov     eax, SYSCALL_READ       ; read function
        mov     ebx, STDIN              ; Arg 1: file descriptor
        mov     ecx, buf                ; Arg 2: address of buffer
        mov     edx, BUFLEN             ; Arg 3: buffer length
        int     080h

        ; error check
        mov     [rlen], eax             ; save length of string read
        cmp     eax, 0                  ; check if any chars read
        jg      read_OK                 ; >0 chars read = OK
        mov     eax, SYSCALL_WRITE      ; ow print error mesg
        mov     ebx, STDOUT
        mov     ecx, msg4
        mov     edx, len4
        int     080h
        jmp     exit                    ; skip over rest

read_OK:
        mov     ecx, buf                ; store pointer to first char
        mov     edi, res                ; store address of location to store converted char
        mov     esi, [rlen]             ; count of number of chars left to read

loop:   nop
        mov     al, [ecx]               ; copy current char to al

        ; if <= 'z'
        cmp     al, 122
        jg      cont

less122: nop
        ; if >= 'a'
        cmp     al, 97
        jge     lcase
        ; if >= 'A'
        cmp     al, 65
        jl      cont

great65: nop
        ; if <= 'Z'
        cmp     al, 90 
        jg      cont

        ; char is between 'A' and 'Z'
ucase:  nop
        add     al, 13                  ; add 13 to the char
        cmp     al, 90
        jbe     cont
        add     al, -26                 ; subtract 26 if needed
        jmp     cont

        ; char is between 'a' and 'z'
lcase:  nop
        add     al, 13                  ; add 13 to the char
        cmp     al, 122
        jbe     cont
        add     al, -26                 ; subtract 26 if needed

        ; continue to the next char
cont:   nop
        mov     [edi], al               ; save changes to the char
        dec     esi                     ; decrement char count
        jz      print                   ; print results if result of decrement is 0
        inc     ecx                     ; advance to next char of buf
        inc     edi                     ; advance to next char of res
        jmp     loop

print:  nop
        ; print "Original: "
        mov     eax, SYSCALL_WRITE      ; write message
        mov     ebx, STDOUT             ; file descriptor
        mov     ecx, msg2               ; pointer to string
        mov     edx, len2               ; specify length of string
        int     080h
        ; print original string (buf)
        mov     eax, SYSCALL_WRITE
        mov     ebx, STDOUT
        mov     ecx, buf
        mov     edx, [rlen]
        int     080h

        ; print "Convert: "
        mov     eax, SYSCALL_WRITE
        mov     ebx, STDOUT
        mov     ecx, msg3
        mov     edx, len3
        int     080h
        ; print converted string (res)
        mov     eax, SYSCALL_WRITE
        mov     ebx, STDOUT
        mov     ecx, res
        mov     edx, [rlen]
        int     080h

        ; final exit
exit:   mov     eax, SYSCALL_EXIT       ; exit function
        mov     ebx, 0                  ; exit code, 0=normal
        int     080h                    ; ask kernel to take over
