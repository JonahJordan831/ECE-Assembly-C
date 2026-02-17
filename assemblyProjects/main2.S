#include <avr/io.h>
.text
.global main

main: NOP
//following the same pattern throughoutfor fibonacci sequence
ldi r16, 0x00
sts 0x200, r16
ldi r16, 0x01
sts 0x201, r16
add r16, r17
sts 0x202, r16

lds r16, 0x201
lds r17, 0x202
add r16, r17
sts 0x203, r16

lds r16, 0x202
lds r17, 0x203
add r16, r17
sts 0x204, r16

lds r16, 0x203
lds r17, 0x204
add r16, r17
sts 0x205, r16

lds r16, 0x204
lds r17, 0x205
add r16, r17
sts 0x206, r16

lds r16, 0x205
lds r17, 0x206
add r16, r17
sts 0x207, r16

lds r16, 0x206
lds r17, 0x207
add r16, r17
sts 0x208, r16

lds r16, 0x207
lds r17, 0x208
add r16, r17
sts 0x209, r16

lds r16, 0x208
lds r17, 0x209
add r16, r17
sts 0x20A, r16

lds r16, 0x209
lds r17, 0x20A
add r16, r17
sts 0x20B, r16

lds r16, 0x20A
lds r17, 0x20B
add r16, r17
sts 0x20C, r16

lds r16, 0x20B
lds r17, 0x20C
add r16, r17
sts 0x20D, r16

lds r16, 0x20C
lds r17, 0x20D
add r16, r17
sts 0x20E, r16