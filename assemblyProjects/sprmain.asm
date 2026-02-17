; AVR Assembly Code for MAX7219 LED Matrix Display with Button Cycle
; Without m328Pdef.inc

.org 0x00
    rjmp start

; -------------------------------
; Initialization Code
; -------------------------------
start:
    ; Set MOSI, SCK, SS as outputs
    sbi DDRB, PB3   ; MOSI (PB3)
    sbi DDRB, PB5   ; SCK  (PB5)
    sbi DDRB, PB2   ; SS   (PB2)

    ; Set button (PD2) as input with pull-up enabled
    cbi DDRD, PD2   
    sbi PORTD, PD2  

    ; Enable SPI in Master Mode, Clock Rate F_CPU/16
    ldi r16, (1 << SPE) | (1 << MSTR) | (1 << SPR0)
    out SPCR, r16

    ; Initialize MAX7219
    call max7219_init

    ; Start main loop
    rjmp main

; -------------------------------
; Main Loop
; -------------------------------
main:
    ldi r17, 0  ; Current sprite index

next_sprite:
    ; Compute sprite address
    ldi ZL, low(stages)
    ldi ZH, high(stages)
    ldi r18, 8      ; Size of each stage (8 bytes per stage)
    mul r17, r18    ; Multiply index by 8
    add ZL, r0      ; Update Z pointer
    adc ZH, r1

    call display_stage

    ; Wait for button press
button_wait:
    sbic PIND, PD2   ; Check if button is pressed
    rjmp button_wait

    ; Debounce delay
    call delay

    ; Check button state again to confirm press
    sbic PIND, PD2
    rjmp button_wait

    ; Increment sprite index
    inc r17
    cpi r17, 4  ; If index > 3, reset to 0
    brlo no_reset
    clr r17
no_reset:

    rjmp next_sprite

; -------------------------------
; Display Stage Routine
; -------------------------------
display_stage:
    ldi r19, 1  ; Start at row 1
    ldi r20, 8  ; Number of rows

send_rows:
    lpm r16, Z+  ; Load row data for the current row
    call max7219_write

    inc r19
    dec r20
    brne send_rows
    ret

; -------------------------------
; MAX7219 Write Routine
; -------------------------------
max7219_write:
    cbi PORTB, PB2    ; CS low (Start)
    out SPDR, r19     ; Send row number

wait_spi1:
    in r16, SPSR      ; Read SPSR
    sbrs r16, SPIF    ; Check SPIF bit
    rjmp wait_spi1

    out SPDR, r16     ; Send row data

wait_spi2:
    in r16, SPSR      ; Read SPSR
    sbrs r16, SPIF    ; Check SPIF bit
    rjmp wait_spi2

    sbi PORTB, PB2    ; CS high (End)
    ret

; -------------------------------
; MAX7219 Initialization Routine
; -------------------------------
max7219_init:
    ldi r16, 0x09    ; Decode mode: No decode
    ldi r18, 0x00
    call max7219_write

    ldi r16, 0x0A    ; Intensity: Medium
    ldi r18, 0x08
    call max7219_write

    ldi r16, 0x0B    ; Scan limit: Use all 8 rows
    ldi r18, 0x07
    call max7219_write

    ldi r16, 0x0C    ; Shutdown mode: Normal operation
    ldi r18, 0x01
    call max7219_write

    ldi r16, 0x0F    ; Display test: Off
    ldi r18, 0x00
    call max7219_write
    ret

; -------------------------------
; Simple Delay Routine
; -------------------------------
delay:
    ldi r20, 255
outer_loop:
    ldi r21, 255
inner_loop:
    dec r21
    brne inner_loop
    dec r20
    brne outer_loop
    ret

; -------------------------------
; Stage Data (4 Stages with 8x8 Pixels Each)
; -------------------------------
.align 2
stages:
    ; Stage 1: All lights off
    .db 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000  ; Stage 1

    ; Stage 2: First pattern
    .db 0b00011000, 0b00111100, 0b01111110, 0b11011011, 0b11111111, 0b00111100, 0b01111110, 0b10100101  ; Stage 2

    ; Stage 3: Second pattern
    .db 0b01000010, 0b00111100, 0b01111110, 0b11011011, 0b11111111, 0b01100110, 0b10100101, 0b10000001  ; Stage 3

    ; Stage 4: Third pattern
    .db 0b00100100, 0b00111100, 0b00111100, 0b01011010, 0b10111101, 0b00111100, 0b01100110, 0b01000010  ; Stage 4
