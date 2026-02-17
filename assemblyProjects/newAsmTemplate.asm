

.org 0x0000
rjmp RESET

RESET:
    ldi r16, 0x01
    out DDRB, r16
loop:
    rjmp loop



