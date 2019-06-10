; Dan Engbert CMSC 313 Spring 2017
;
; Takes an input string from the user and encodes any escape sequences
; (including octal escape sequences)

%define STDIN 0
%define STDOUT 1
%define SYSCALL_EXIT  1
%define SYSCALL_READ  3
%define SYSCALL_WRITE 4
%define BUFLEN 256


        ; uninitialized data section (allocates memory)
        SECTION .bss
buf:    resb BUFLEN                     ; buffer for inputed string
res:    resb BUFLEN                     ; converted string (has same length)
rlen:   resb 4                          ; length of string(s)


        ; initialized data section
        SECTION .data
; null terminated strings:
msg1:   db "Enter string: ", 0          ; user prompt message
len1:   equ $-msg1                      ; length of first message
msg2:   db "Original: ", 0              ; original string label
msg3:   db "Convert:  ", 0              ; converted string label
msg4:   db "Read error", 10, 0      ; error message
msg5:   db "Error: unknown escape sequence \X", 10, 0
msg6:   db "Error: octal value overflow!", 10, 0

map:    db 7                            ; what 'a' maps to
        db 8                            ; 'b'
        db -1                           ; 'c'
        db -1                           ; 'd'
        db -1                           ; 'e'
        db 12                           ; 'f'
        db -1                           ; 'g'
        db -1                           ; 'h'
        db -1                           ; 'i'
        db -1                           ; 'j'
        db -1                           ; 'k'
        db -1                           ; 'l'
        db -1                           ; 'm'
        db 10                           ; 'n'
        db -1                           ; 'o'
        db -1                           ; 'p'
        db -1                           ; 'q'
        db 13                           ; 'r'
        db -1                           ; 's'
        db 9                            ; 't'
        db -1                           ; 'u'
        db 11                           ; 'v'
        db -1                           ; 'w'
        db -1                           ; 'x'
        db -1                           ; 'y'
        db -1                           ; 'z'
        ; note: backslash maps to backslash


        ; code section
        SECTION .text
        global  _start                  ; let loader see entry point
_start: nop                             ; entry point
        ; prompt user for input
        mov     ecx, msg1               ; Arg2: addr of message
        call    print_string

        ; read user input
        mov     eax, SYSCALL_READ       ; read function
        mov     ebx, STDIN              ; Arg 1: file descriptor
        mov     ecx, buf                ; Arg 2: address of buffer
        mov     edx, BUFLEN             ; Arg 3: buffer length
        dec     edx                     ; ensure room for null terminator later
        int     080h

        ; null terminate the inputed string
        mov     esi, buf
        mov     bx, 0
        mov     [esi + eax], bx         ; null terminate the string
        inc     eax                     ; now the string is 1 byte longer

        ; error check
        mov     [rlen], eax             ; save length of string read
        cmp     eax, 1                  ; check if any chars read
        jg      read_OK                 ; >0 chars read = OK
        mov     ecx, msg4               ; ow print error mesg
        call    print_string
        jmp     exit                    ; skip over rest

read_OK:
        mov     esi, buf                ; store pointer to first char
        mov     edi, res                ; store address of location to store converted char
        mov     ebp, map                ; store address of map location
        mov     ebx, 0

; loop over all chars in string until null char is reached
; string is guarenteed to end with a null terminator at this point
loop:   nop
        ; while current char != 0
        mov     al, [esi]               ; copy current char to al
        cmp     al, 0                   ; stop when null char is reached
        mov     [edi], al               ; initial copy of char to result string
        jne     next
        jmp     print

next:   nop
        ; if char is '\'
        cmp     al, 92
        jne     cont
        inc     esi                     ; advance to next char
        ; don't increment edi because we can overwrite the '\' copied already there
        call    handle_ESC

        ; continue to the next char
cont:   nop
        mov     [edi], al               ; save changes to the char
        inc     esi                     ; advance to next char of buf
        inc     edi                     ; advance to next char of res
        jmp     loop



; First check if the char after the slash is "a-z"
; Have a table of 26 bytes mapping each escape character to its escape sequence

; for:
;   You can do following with MUL, or something easier:
;    total_value = (total_value * 8) + digit_value;
;    (we're multiplying by a power of 2 so do something easier
;    like a binary shift)
; multiply by 8 with shl eax, 3
handle_ESC: nop
        mov     al, [esi]               ; copy current char to al
        mov     [edi], al               ; initial copy of char to result string
        cmp     al, 0                   ; stop when null char is reached
        jne     p1
        jmp     print

        ; if >= 'z'
p1:     cmp     al, 122
        ja      notlow
        ; and if <= 'a'
        cmp     al, 97
        jb      notlow

        ; char is lowercase letter
        ; replace value with value in map (if not -1)
        mov     bl, al
        add     bl, -97
        mov     bl, [ebp + ebx]
        cmp     bl, -1
        je      bad_seq
        mov     al, bl                  ; allow main loop to save the changes to al
        jmp     hdone

        ; invalid escaped character so print error message
bad_seq: nop
        mov     ecx, msg5               ; pointer to string
        ; add current char to error string
        mov     [ecx + 32], al
        call    print_string
        mov     al, '\'
        jmp hdone

oct_over: nop
        mov     ecx, msg6               ; pointer to string
        call    print_string
        mov     al, '\'
        jmp hdone

        ; char is not a lowercase letter
notlow: nop
        ; check if char is another backslash
        cmp     al, '\'
        je      hdone
        ; check if it's an octal number (i.e. between '0' and '7')
        mov     cl, 0                   ; cl = (decimal) total so far
        mov     dl, 0                   ; number of nums we've encountered so far

        ; NUMBER LOOP:::
nloop:  nop
        cmp     al, 48
        jb      ndone
        cmp     al, 55
        ja      ndone

        ; it's an octal number:
        inc     dl
        add     al, -48                 ; convert from ascii num char to octal int
        cmp     cl, 0
        jne     nagain

        ; if this is the first number found
nfirst: mov     cl, al
        jmp     nloop_btm

        ; when not the first number found
nagain: nop
        ; left shift 3 times to multiply by 8
        ; check that the decimal value is <= 127
        shl     cl, 3
        jc      oct_over
        add     cl, al
        jo      oct_over
        cmp     cl, 127
        ja      oct_over

        ; bottom of nloop
nloop_btm: nop
        cmp     dl, 3
        jae     ndone
        inc     esi                     ; advance to next char of buf
        mov     al, [esi]               ; copy current char to al
        cmp     al, 0                   ; stop when null char is reached
        jne     nloop

        ; done with the escaped octal numbers
ndone:  nop
        cmp      dl, 0
        je       bad_seq
        cmp      dl, 3
        jae      ndone2
decr:   dec      esi

ndone2: mov      al, cl                 ; allow main loop to save changes to al

        ; end of handle_ESC
hdone:  ret
        ; TODO: make sure this^ won't cause cont to skip the null char



print:  nop
        ; print "Original: "
        mov     ecx, msg2               ; pointer to string
        call    print_string

        ; print original string (buf)
        mov     ecx, buf
        call    print_string

        ; print "Convert: "
        mov     ecx, msg3
        call    print_string

        ; print converted string (res)
        mov     ecx, res
        call    print_string
        jmp exit

; ecx should contain string pointer
; prints a null terminated string
print_string: nop
        ; TODO: store registers somewhere and retrieve them later (like the stack)
        ; (only push registers you're using!)
        ; determine string length
        push    eax
        push    ebx
        push    edx
        push    esi
        push    ebp

        mov     edx, 0                  ; string length (including null terminator)
        mov     eax, ecx                ; pointer to first char
ln_loop: nop
        mov     bl, [eax]               ; copy current char to bl
        inc     edx                     ; increment string length
        inc     eax                     ; advance pointer by one char (byte)
        cmp     bl, 0
        jne     ln_loop

        ; print the string now that:
        ; ecx contains addr of string and edx contains string length
        mov     eax, SYSCALL_WRITE      ; write message
        mov     ebx, STDOUT             ; file descriptor
        int     080h

        pop     ebp
        pop     esi
        pop     edx
        pop     ebx
        pop     eax

        ret

        ; final exit
exit:   mov     eax, SYSCALL_EXIT       ; exit function
        mov     ebx, 0                  ; exit code, 0=normal
        int     080h                    ; ask kernel to take over
