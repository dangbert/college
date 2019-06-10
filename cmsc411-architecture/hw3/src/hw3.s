@ Dan Engbert - CMSC 411, Fall 17
@ HW3: multiply arrays
@ run this program in ARMSim


@ *************** Data Segment ***************
	.data
arrA:   .byte   1, 2, 3, 4
arrB:   .byte   9, 8, 7, 6
@ TODO: could also make res a huge array and then the size to use could be chosen later
res:    .byte   0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0


@ *************** Code Segment ***************
	.equ	SWI_Write, 0x05
	.equ	SWI_Exit,  0x18

	.text
_start:
    @ store address of arrays in registers r0 and r1
    ldr     r0, =arrA
    ldr     r1, =arrB
    ldr     r2, =4           @ length of arrays
    ldr     r11, =res        @ result array

    @ multiply A * B (results is 1x1 matrix)
    ldr     r3, =0           @ current position in array
    ldr     r7, =0           @ result value

loop:
    ldrb    r4, [r0, r3]     @ store element of arrA at position #r3 in r4
    ldrb    r5, [r1, r3]     @ store element of arrB at position #r3 in r5

    mul     r6, r4, r5       @ multiply the two elements
    add     r7, r6           @ add the result to the running total
    add     r3, #1           @ increment index
    cmp     r3, r2
    bne     loop             @ loop until we reach the end of the array
    @ resulting value is now stored in r7


    @ multiply B * A (results is 4x4 matrix, stored in res)
    ldr     r8, =0           @ current position in arrB
loopB:
    ldr     r3, =0           @ current position in arrA
    ldrb    r5, [r1, r8]     @ store element of arrB at position #r8 in r5

loopA:
    ldrb    r4, [r0, r3]     @ store element of arrA at position #r3 in r4

    @ store target index in result array in r9
    mov     r9, r8
    mul     r9, r2
    add     r9, r3

    @ multiply and store result in res array
    mov     r10, r5
    mul     r10, r4
    strb    r10, [r11, r9]

    add     r3, #1
    cmp     r3, r2
    bne     loopA

    add     r8, #1
    cmp     r8, r2
    bne     loopB


    @ break here and use view>memory in ARMSim to view the hex values stored in the res arary (address in r11)
    @ each element of the array is 1 byte (2 hex characters)
    @ add a break after each value is set above to see how the array is populated
    nop

    @ exit
    mov     r0, #SWI_Exit
	mov	r1, #0
	swi	0x123456
