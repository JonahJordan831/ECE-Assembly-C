;BUTTON
    
;STACK POINTER
    
    LDI R16, HIGH(RAMEND)
    OUT SPH, R16
    LDI R16, LOW (RAMEND)
    OUT SPL, R16
    
    ;CONFIGURE IO
    
    ;LED ON PB 5 AND BUTTON ON PB4
    
    LDI R16, 0XEF
 
    
    
    
    
    
    
    
    


