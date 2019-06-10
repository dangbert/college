@ RJ Joyce
@ Daniel Engbert (do e^x)
@ Arvin Siva
@ Amit Singh
@ CMSC 411
@ 12/03/2017

@ https://people.ece.cornell.edu/land/courses/ece4760/Math/cordic.c
@ Results are stored as 32-bit words in 16.16 fixed point.
@ The upper 16 bits are the whole part of the number
@ The lower 16 bits are the fractional part
@ Divide the lower 16 bits by 65536 to convert it

@ *************** Code Segment ***************

	.text
	
@ Set the value of X here
@ if x > 11.09034 then exp won't work because of a 16 bit left shift will zero the answer
.target_angle:
	.single	0e10.53         @ 0e is the encoding for a floating point (like 0x for hex)

@ Results go here
.results:
	.word	0				@ Sin
	.word	0				@ Cos
	.word	0				@ Tan
	.word	0				@ Exp

.ag_const:
	.word	39796			@ AG_CONST = 0.6072529350 * 65536.0

.exponent_const:
	.word 	2139095040

.mantissa_const:
	.word	8388607

.highest_bit_const:
	.word	2147483648

.sin_angles:
	.word 	2949120
	.word 	1740963
	.word 	919876
	.word 	466945
	.word 	234378
	.word 	117303
	.word 	58666
	.word 	29334
	.word 	14667
	.word 	7333
	.word 	3666
	.word 	1833

.sinh_angles:
	.word	35999
	.word 	16739
	.word	8235
	.word	4101
	.word	2049
	.word	1024
	.word	512
	.word	256
	.word	128
	.word	64
	.word	32
	.word	16

@ values of k for e^x
.expk_vals:
    .word   726817
    .word   363409 @ ln(256) * 2^16 (converts to fixed point)
    .word   181705 @ ln(16) * 2^16
    .word   90852
    .word   45426
    .word   26573  @ ln(3/2) * 2^16
    .word   14624
    .word   7719   @ ln(9/8) * 2^16
    .word   3973
    .word   2017   @ ln(33/32) * 2^16
    .word   1016
    .word   510    @ ln(129/128) * 2^16
    .word   256
    .word   128
    .word   64
    .word   32
    .word   16
    .word   8
    .word   4
    .word   2
    .word   1

@ exponent vals (for calculating e^k and then multiplying y by that)
@ (just using .word to make the indexing the same as .expk_vals)
.expn_vals:
    .word   16 @ this one is included really just to make the answer 0 if x >= (726817)
    .word   8  @ for 2^8    y * 2^8 = y << 8   (left shift)
    .word   4  @ for 2^4            = y << 4
    .word   2  @ for 2^2
    .word   1  @ for 2^1            = y << 1
    .word   1  @ for 1 + 2^-1       = y + (y >> 1)  (index = 5, pattern changes)
    .word   2  @ for 1 + 2^-2       = y + (y >> 2)
    .word   3  @ for 1 + 2^-3       = y + (y >> 3)
    .word   4  @ for 1 + 2^-4       = y + (y >> 4)
    .word   5
    .word   6
    .word   7
    .word   8
    .word   9
    .word   10
    .word   11
    .word   12
    .word   13
    .word   14
    .word   15
    .word   16

@ Main subroutine
_start:
	@ Convert target_angle from ieee 754 to 16.16 fixed point
	vldr	s0, .target_angle
	vmov		r13, s0			@ exponent
	vmov		r12, s0			@ mantissa
	ldr		r11, .exponent_const
	ldr		r10, .mantissa_const
	ldr		r9, .highest_bit_const

	and		r13, r11		@ exponent & exponent_const
	and		r12, r10		@ mantissa & mantissa_const
	lsr		r13, #23		@ exponent >>= 23
	sub		r13, r13, #111	@ exponent = exponent - 127 + 16
	lsl		r12, #9			@ mantissa <<= 9

	mov		r14, #1			@ target_angle = 1
	mov		r11, #0			@ count = 0

fp_forloop:

	mov		r10, r12        @ mantissa
	and		r10, r9			@ highest_bit = mantissa & highest_bit_const
	lsr		r10, #31		@ highest_bit >>= 31
	lsl		r14, #1			@ target_angle <<= 1
	add		r14, r10		@ target_angle += highest_bit
	lsl		r12, #1			@ mantissa <<= 1
	add		r11, #1			@ count ++

	cmp		r11, r13		@ if count < exponent
	blt		fp_forloop

    @b sub_expx @ (code is getting stuck at line 200) TODO: remove this

@ Subroutine to calculate sin(x) and cos(x)
@ (see p. 50 in lecture 11)
sub_sinx_cosx:

	@ x is passed in through r14
	@ Do not modify r14 in this subroutine

	@ Initialize variables

	ldr		r13, .ag_const	@ X = AG_CONST
	mov		r12, #0			@ Y = 0
	mov		r11, #0			@ CurrAngle = 0
	mov		r10, #0			@ Step = 0

sin_cos_forloop:
	cmp		r14, r11		@ if TargetAngle > CurrAngle
	blt		sin_cos_less

	mov		r9, r13
	mov		r8, r12
	asr		r8, r10
	sub		r9, r8			@ NewX = X - (Y >> Step)

	mov		r7, r13
	asr		r7, r10
	add		r12, r12, r7	@ Y = (X >> Step) + Y

	mov		r13, r9			@ X = NewX

	ldr		r9, =.sin_angles
	mov		r8, r10
	lsl		r8, #2
	ldr		r7, [r9, r8]
	add		r11, r11, r7	@ CurrAngle += Angles[Step]

	b		sin_cos_join

sin_cos_less:
	mov		r9, r13
	mov		r8, r12
	asr		r8, r10
	add		r9, r8			@ NewX = X + (Y >> Step)

	mov		r7, r13
	asr		r7, r10
	sub		r12, r12, r7	@ Y = Y - (X >> Step)

	mov		r13, r9			@ X = NewX

	ldr		r9, =.sin_angles
	mov		r8, r10
	lsl		r8, #2
	ldr		r7, [r9, r8]
	sub		r11, r11, r7	@ CurrAngle -= Angles[Step]

sin_cos_join:
	add		r10, r10, #1	@ Step += 1
	cmp		r10, #12		@ if Step < 12
	ble		sin_cos_forloop	@ branch to sin_cos_forloop

	ldr		r11, =.results	
	str		r12, [r11]		@ Store the result of sin(x)
	str		r13, [r11, #4]	@ Store the result of cos(x)

@ Subroutine to calculate tan(x)
sub_tanx:

	@ x is passed in through r14
	@ Do not modify r14 in this subroutine
	@ TODO: Calculations

	mov		r13, #0				@ Put the result in r13 - using 0 as a placeholder
	ldr		r11, =.results	
	str		r13, [r11, #8]		@ Store the result of tan(x)


@ Subroutine to calculate e^x
@ x is passed in through r14 (in the 16.16 format)
@ if x > 11.09034 then e^x won't work because a 16 bit left shift will zero the answer
@ (to get the final answer in decimal you have to divide it by 2^16)
@ algorithm came from:
@ https://www.quinapalus.com/efunc.html
@ http://lipas.uwasa.fi/~TAU/AUTO3330/slides.php?Mode=Printer&File=1300Ari.txt&MicroExam=Off&Images=Off&Menu=Off
sub_expx:
    ldr     r13, =.expk_vals    @ array of constants (k)
    ldr     r5,  =.expn_vals    @ array for (calculating) (e^k)
    mov     r1, #-4             @ array index (note: array length is 11)
    mov     r9, r14             @ x
    mov     r12, #65536         @ y   = 1 * 2^16 (will become the final value)

    @ advance index until x < k
ex_advance:
    add     r1, #4              @ increment index (each word is 4 bytes)
    @ check that we haven't reached the end of the array (if we did we're done)
    cmp     r1, #84
    beq     ex_done             @ reached end of array

    ldr     r2, [r13, r1]       @ current k
    cmp     r2, r9              @ check if k < x
    blt     ex_ready

    @ increment index
    b       ex_advance

@ now x < k (k should be as large as possible)
@ (ready to use the value of k)
ex_ready:
    @ r2 now contains the value of k to use (at index r1)
    @ now set x = x - k
    sub     r9, r2

    @ now multiply y and e^k:
    ldr     r2, [r5, r1]        @ exponent val
    cmp     r1, #20             @ at index >= 5 the pattern for e^k changes
    bge     ex_case2

ex_case1: @ index < 6
    @ note: Y * 2^n  =  y << n
    @ current y is in r12
    lsl     r12, r2             @ left shift
    b       ex_advance

ex_case2: @ index >= 6
    @ note: y * (1 + 2^-n)  =  y + (y >> n)
    mov     r3, r12
    lsr     r3, r2              @ right shift
    add     r12, r3
    b       ex_advance

ex_done:
	mov		r13, r12			@ Put the result in r13
	ldr		r11, =.results	
	str		r13, [r11, #12]		@ Store the result of e^x
	.end
