#include <P18F4620.inc>
    config OSC = INTIO67
    config WDT = OFF
    config LVP = OFF
    config BOREN = OFF

; Variable declarations
InA     equ     0x20
Result  equ     0x22

    ORG     0x0000

START:
    MOVLW   0x0F            ; Load 0x0F into WREG
    MOVWF   ADCON1          ; ADCON1 = 0x0F

    MOVLW   0xFF            ; Load 0xFF into WREG
    MOVWF   TRISA           ; Set TRISA to 0xFF (PORTA as input)
    MOVWF   TRISB           ; Set TRISB to 0xFF (PORTB as input)
    MOVWF   TRISE           ; Set TRISE to 0xFF (PORTE as input)

    MOVLW   0x00            ; Load 0x00 into WREG
    MOVWF   TRISC           ; Set TRISC to 0x00 (PORTC as output)
    MOVWF   TRISD           ; Set TRISD to 0x00 (PORTD as output)

MAIN_LOOP:
    MOVF    PORTA, W        ; Read PORTA into WREG
    ANDLW   0x0F            ; Mask out upper 4 bits (keep lower 4 bits)
    MOVWF   InA             ; Store the result in InA

    ; Compare InA with 0x09
    MOVLW   0x09            ; Load 0x09 into WREG
    CPFSGT  InA             ; Compare InA with WREG (0x09)
    GOTO    LESS_OR_EQUAL   ; If InA <= 9, jump to LESS_OR_EQUAL

    ; If InA > 9, add 6 to InA
    MOVLW   0x06            ; Load 0x06 into WREG
    ADDWF   InA, W          ; Add 6 to InA (W = InA + 6)
    MOVWF   Result          ; Store the result in Result
    GOTO    OUTPUT          ; Jump to OUTPUT

LESS_OR_EQUAL:
    ; If InA <= 9, no change is needed
    MOVF    InA, W          ; Load InA into WREG
    MOVWF   Result          ; Store the result in Result

OUTPUT:
    MOVFF   Result, PORTC   ; Copy Result to PORTC

    GOTO    MAIN_LOOP	    ; Jump to main loop

    END