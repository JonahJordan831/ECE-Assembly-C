#include <avr/io.h>
.text
.global main


.org 0x0000
rjmp RESET             ; Reset vector

RESET:
  
    ; Load input half-precision floats from SRAM (0x0200 - 0x0203)
    lds r16, 0x0200     ; Operand A low byte
    lds r17, 0x0201     ; Operand A high byte
    lds r18, 0x0202     ; Operand B low byte
    lds r19, 0x0203     ; Operand B high byte

    ; === Extract sign bits ===
    mov r20, r17
    andi r20, 0x80         ; sign_A

    mov r21, r19
    andi r21, 0x80         ; sign_B

    ; === Extract exponents (bits 14?10 of half float) ===
    mov r22, r17
    swap r22
    andi r22, 0x1F         ; exp_A
    mov r23, r19
    swap r23
    andi r23, 0x1F         ; exp_B

    ; === Extract mantissas (10 bits) and add implicit 1 ===
    andi r17, 0x03         ; keep only bits 1:0
    lsl r17
    rol r16
    ldi r25, 0x04          ; Add implicit 1 in bit 10
    or r17, r25
    mov r24, r16           ; r25:r24 = mantissa_A

    andi r19, 0x03
    lsl r19
    rol r18
    ldi r27, 0x04
    or r19, r27
    mov r26, r18           ; r27:r26 = mantissa_B
    ; === Align exponents ===
    cp r22, r23
    breq DO_ADD_SUB        ; If exponents match, continue

    brlt SHIFT_B           ; If exp_A < exp_B, shift B
SHIFT_A:
    sub r22, r23
SHIFT_A_LOOP:
    lsr r27
    ror r26
    dec r22
    brne SHIFT_A_LOOP
    rjmp DO_ADD_SUB

SHIFT_B:
    sub r23, r22
SHIFT_B_LOOP:
    lsr r25
    ror r24
    dec r23
    brne SHIFT_B_LOOP

DO_ADD_SUB:
    mov r30, r22           ; r30 = aligned exponent

    cp r20, r21
    breq ADDITION          ; Same signs ? add

    ; === Subtraction ===
    cp r25, r27
    brlo SWAP_FOR_SUB
    cp r24, r26
    brlo SWAP_FOR_SUB

SUBTRACT:
    mov r28, r24
    sub r28, r26
    mov r29, r25
    sbc r29, r27
    mov r31, r20           ; result sign = A
    rjmp NORMALIZE

SWAP_FOR_SUB:
    mov r28, r26
    sub r28, r24
    mov r29, r27
    sbc r29, r25
    mov r31, r21           ; result sign = B
    rjmp NORMALIZE

; === Addition ===
ADDITION:
    mov r28, r24
    add r28, r26
    mov r29, r25
    adc r29, r27
    mov r31, r20           ; result sign = A
    rjmp NORMALIZE

; === Normalize so leading 1 is in bit 10 ===
NORMALIZE:
    ldi r16, 0x00          ; shift counter
NORMALIZE_LOOP:
    lsl r29
    rol r28
    inc r16
    sbrs r29, 2            ; bit 10 = bit 2 of r29
    rjmp NORMALIZE_LOOP
    sub r30, r16           ; adjust exponent

; === Pack result: sign (bit 15), exponent (14-10), mantissa (9-0) ===
PACK_RESULT:
    andi r29, 0x03         ; clear upper bits
    ror r29
    ror r28

    swap r30              ; exponent in upper nibble
    andi r30, 0xF8
    or r29, r30            ; OR in exponent
    or r29, r31            ; OR in sign

    ; Store result in memory: 0x0204 (sum low), 0x0205 (sum high)
    sts 0x0204, r28
    sts 0x0205, r29

    ; Also store to 0x0206 / 0x0207 for diff (or reuse later)
    sts 0x0206, r28
    sts 0x0207, r29
    rjmp .


