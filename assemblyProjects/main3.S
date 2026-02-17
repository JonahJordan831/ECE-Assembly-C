#include <avr/io.h>
.text
.global main

main:
    ldi r16, 0x02         ; Start candidate at 2
    ldi r17, 0x00         ; Address offset (not needed)
    ldi r30, 0x00         ; Set low byte to 0x00
    ldi r31, 0x02         ; Set high byte to 0x02 because its a 16-bit system

next_candidate:
    ldi r18, 0x02         ; Start divisor from 2
    ldi r19, 0x00         ; Flag (0 = prime, 1 = not prime)

check_division:
    cp r18, r16           ; If divisor == candidate, number is prime
    breq store_prime

    mov r20, r16          ; Copy candidate to r20 for division
division_loop:
    sub r20, r18          ; Subtract divisor from candidate
    brcs next_divisor     ; If borrow (negative result), move to next divisor
    brne division_loop    ; Continue division if remainder != 0

    ldi r19, 1            ; Mark as not prime

next_divisor:
    inc r18               ; Increment divisor
    cp r18, r16           ; Stop when divisor == candidate
    brne check_division

store_prime:
    cpi r19, 0            ; If flagged as non-prime, skip storing
    brne next_number

    st Z+, r16             ; Store prime number in memory at Z
                 ; Increment low byte of Z register (gave error when I tried to inc)
    brne next_candidate    ; inc reg 30 had to do Z+)
                            ; If low byte overflowed, increment high byte of Z
next_number:
    inc r16               ; Increment prime candidate
    cpi r16, 255          ; Stop at 255
    brne next_candidate
