;
; things to remember 1 sign bit, 5 exponent bits, 10 mantissa bits
; Inputs i chose to test: A = 0x3E00 (1.5), B = 0x4200 (2.5) (these can be changed if you want
; Memory Map from prompt
;   0x0200/0x0201 - Operand A (low/high)
;   0x0202/0x0203 - Operand B (low/high)
;   0x0204/0x0205 - Sum (low/high)
;   0x0206/0x0207 - Difference (low/high)

.org 0x0000
	rjmp start

.org 0x003C
	rjmp done

start:
	; Store A = 0x3E00 (1.5) can be changed
	ldi r16, 0x00
	sts 0x0200, r16
	ldi r16, 0x3E
	sts 0x0201, r16

	; Store B = 0x4200 (2.5)
	ldi r16, 0x00
	sts 0x0202, r16
	ldi r16, 0x42
	sts 0x0203, r16

	; Load A
	lds r16, 0x0200 ; low
	lds r17, 0x0201 ; high

	; Load B
	lds r18, 0x0202 ; low
	lds r19, 0x0203 ; high

; extracting  the first A field
	mov r20, r17        ; Copy A high byte
	andi r20, 0x80      ; signA in r20

	mov r21, r17
	andi r21, 0x7C
	lsr r21
	lsr r21
	lsr r21            ; exponent A in r21

	mov r22, r17
	andi r22, 0x03
	swap r22
	or r16, r22
	mov r22, r16       ; fracA (low) in r22, will extend in r23

	clr r23            ; fracA high part

; Extract fields from B 
	mov r24, r19
	andi r24, 0x80      ; signB in r24

	mov r25, r19
	andi r25, 0x7C
	lsr r25
	lsr r25
	lsr r25            ; exponentB in r25

	mov r26, r19
	andi r26, 0x03
	swap r26
	or r18, r26
	mov r26, r18       ; fracB (low) in r26, -- will extend in r27

	clr r27            ; fracB high part

;  Aligning the exponents for addition 
	cp r21, r25
	brge align_A_to_B

align_B_to_A:
	sub r25, r21
	mov r28, r25
align_B_loop:
	lsr r26    ; loops that do the aligning
	ror r27
	dec r28
	brne align_B_loop
	rjmp do_add

align_A_to_B:
	sub r21, r25
	mov r28, r21
align_A_loop:
	lsr r22
	ror r23
	dec r28
	brne align_A_loop

do_add:
	add r22, r26
	adc r23, r27

	; Normalizing the result
	ldi r28, 0
norm_add_loop:
	tst r23
	brmi norm_add_done
	lsl r22
	rol r23
	inc r28
	rjmp norm_add_loop

norm_add_done:
	sub r21, r28       ; adjust exponent

	; Packing the result
	mov r24, r23
	andi r24, 0xC0
	lsr r24
	lsr r24
	lsr r24
	lsr r24
	lsr r24
	lsr r24            ; top 2 bits of fractopm

	lsl r21
	lsl r21
	lsl r21
	andi r21, 0x7C     ; shift the exponent to proper position
	or r21, r24        ; combine with top fraction bits

	sts 0x0204, r22    ; Sum low
	sts 0x0205, r21    ; Sum high

; SUBTRACTION
	; Reload operands
	lds r16, 0x0200
	lds r17, 0x0201
	lds r18, 0x0202
	lds r19, 0x0203

	; Re-extract A
	mov r21, r17
	andi r21, 0x7C
	lsr r21
	lsr r21
	lsr r21

	mov r22, r17
	andi r22, 0x03
	swap r22
	or r16, r22
	mov r22, r16
	clr r23

	; Re-extract B
	mov r25, r19
	andi r25, 0x7C
	lsr r25
	lsr r25
	lsr r25

	mov r26, r19
	andi r26, 0x03
	swap r26
	or r18, r26
	mov r26, r18
	clr r27

	; Compare magnitudes
	clr r30   ; sign = 0 by default
	cp r21, r25
	brlo swap_sub
	breq check_frac_high
	rjmp align_sub

check_frac_high:
	cp r23, r27
	brlo swap_sub
	breq check_frac_low
	rjmp align_sub

check_frac_low:
	cp r22, r26
	brlo swap_sub
	rjmp align_sub

swap_sub:
	; Swap A and B
	mov r30, r21
	mov r21, r25
	mov r25, r30

	mov r30, r22
	mov r22, r26
	mov r26, r30

	mov r30, r23
	mov r23, r27
	mov r27, r30

	ldi r30, 0x80  ; set negative sign

align_sub:
	sub r25, r21
	mov r28, r25
align_sub_loop:
	lsr r26
	ror r27
	dec r28
	brne align_sub_loop

	sub r22, r26
	sbc r23, r27

	; Normalize
	ldi r28, 0
norm_sub_loop:
	tst r23
	brmi norm_sub_done
	lsl r22
	rol r23
	inc r28
	rjmp norm_sub_loop

norm_sub_done:
	sub r21, r28

	; Pack result
	mov r24, r23
	andi r24, 0xC0
	lsr r24
	lsr r24
	lsr r24
	lsr r24
	lsr r24
	lsr r24

	lsl r21
	lsl r21
	lsl r21
	andi r21, 0x7C
	or r21, r24
	or r21, r30   ; include sign bit if negative

	sts 0x0206, r22 ; Diff low
	sts 0x0207, r21 ; Diff high

done:
	rjmp done

	
	;the addition provede close and belivable answers that round to being correct