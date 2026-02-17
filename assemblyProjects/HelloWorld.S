#include <avr/io.h>
.text ; begining
.global main ;entry point

main: NOP ; nop = nothing
ldi R16, 'H'   ; Loading the ascii values into register files and incrementing the address
sts 0x200, R16  ; so it doesnt overwrite the previous ascii
ldi R16, 'e'   
sts 0x201, R16  
ldi R16, 'l'   
sts 0x202, R16  
ldi R16, 'l'  
sts 0x203, R16  
ldi R16, 'o'  
sts 0x204, R16  
ldi R16, ','   
sts 0x205, R16 
ldi R16, ' '   
sts 0x206, R16  
ldi R16, 'W'   
sts 0x207, R16  
ldi R16, 'o'   
sts 0x208, R16  
ldi R16, 'r'   
sts 0x209, R16  
ldi R16, 'l'   
sts 0x20A, R16  
ldi R16, 'd'   
sts 0x20B, R16  
ldi R16, '!'   
sts 0x20C, R16  
