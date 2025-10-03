#include <P18F4620.inc>

config OSC = INTIO67 
config WDT = OFF 
config LVP = OFF 
config BOREN = OFF 


Array1_Start equ 0x40   ; Starting address for array1 (LED D1)
Array2_Start equ 0x50   ; Starting address for array2 (LED D2)

Delay1  equ  0xFF 
Delay2  equ  0XFF 
Counter_L   equ 0x20 
Counter_H   equ 0x21 
Color_Value equ 0x28 
Loop_Count  equ 0x29 

ORG 0x0000 

START: 
    MOVLW 0x0F      ; Load 0x0F into WREG
    MOVWF ADCON1    ; Set ADCON1 to 0x0F (digital mode)

    MOVLW 0x00      ; Load 0x00 into WREG
    MOVWF TRISC     ; Set TRISC to 0x00 (PORTC as output)
    MOVWF TRISD     ; Set TRISD to 0x00 (PORTD as output)

    ; Initialize Array1 (LED D1) in memory locations 0x40 to 0x47
    MOVLW 0x05	           ; Load first value (5 - Purple)
    MOVWF Array1_Start     ; 0x40
    MOVLW 0x03             ; Load second value (3 - Yellow)
    MOVWF Array1_Start + 1 ; 0x41
    MOVLW 0x04             ; Load third value (4 - Blue)
    MOVWF Array1_Start + 2 ; 0x42 ...
    MOVLW 0x06             ; Load fourth value (6 - Cyan)
    MOVWF Array1_Start + 3 
    MOVLW 0x01             ; Load fifth value (1 - Red)
    MOVWF Array1_Start + 4 
    MOVLW 0x02             ; Load sixth value (2 - Green)
    MOVWF Array1_Start + 5 
    MOVLW 0x07             ; Load seventh value (7 - White)
    MOVWF Array1_Start + 6 
    MOVLW 0x00             ; Load eighth value (0 - No Light/Off)
    MOVWF Array1_Start + 7 

    ; Initialize Array2 (LED D2) in memory locations 0x50 to 0x57
    ; Shift values left by 3 bits to align with PORTD bits 3-5
    MOVLW 0x06 << 3	   ; Load first value (6 shifted left by 3 - Cyan)
    MOVWF Array2_Start     ; 0x50
    MOVLW 0x03 << 3	   ; Load second value (3 shifted left by 3 - Yellow)
    MOVWF Array2_Start + 1 ; 0x51
    MOVLW 0x07 << 3	   ; Load third value (7 shifted left by 3 - White)
    MOVWF Array2_Start + 2 ; 0x52 ...
    MOVLW 0x00 << 3	   ; Load fourth value (0 shifted left by 3 - No Light/Off)
    MOVWF Array2_Start + 3 
    MOVLW 0x02 << 3	   ; Load fifth value (2 shifted left by 3 - Green)
    MOVWF Array2_Start + 4 
    MOVLW 0x05 << 3	   ; Load sixth value (5 shifted left by 3 - Purple)
    MOVWF Array2_Start + 5 
    MOVLW 0x04 << 3	   ; Load seventh value (4 shifted left by 3 - Blue)
    MOVWF Array2_Start + 6 
    MOVLW 0x01 << 3	   ; Load eighth value (1 shifted left by 3 - Red)
    MOVWF Array2_Start + 7 

    
    LFSR 0, Array1_Start   ; Initialize FSR0 to point to Array1 (LED D1)
    LFSR 1, Array2_Start   ; Initialize FSR1 to point to Array2 (LED D2)

WHILE_LOOP:	       ; While loop
    MOVLW 0x08         ; Load 0x08 into WREG
    MOVWF Loop_Count   ; Set Loop_Count to 0x08

FOR_LOOP:	       ; For loop
    MOVF INDF0, W      ; Move value pointed by FSR0 into WREG
    MOVWF PORTC        ; Output WREG to PORTC (LED D1)

    MOVF INDF1, W      ; Move value pointed by FSR1 into WREG
    MOVWF PORTD        ; Output WREG to PORTD (LED D2)

    
    CALL DELAY_ONE_SEC ; 1 second delay

    
    INCF FSR0L, F      ; Increment FSR0L (low byte of FSR0)
    INCF FSR1L, F      ; Increment FSR1L (low byte of FSR1)

    ; Decrement Loop_Count and check if it's zero
    DECF Loop_Count, F ; Decrement Loop_Count
    BNZ FOR_LOOP       ; If Loop_Count is not zero, repeat for loop

    ; Reset FSR0 and FSR1 to the start of the arrays
    LFSR 0, Array1_Start 
    LFSR 1, Array2_Start 

    
    GOTO WHILE_LOOP    ; Jump to while loop

; *****************************************************************************
; *                          DELAY_ONE_SEC Subroutine                         *
; * This subroutine generates a delay of approximately 1 second using nested  *
; * loops. The delay is controlled by the constants Delay1 and Delay2.        *
; *****************************************************************************
DELAY_ONE_SEC: 
    MOVLW  Delay1      ; Load constant Delay1 into WREG
    MOVWF  Counter_H   ; Set Counter_H to Delay1
    
LOOP_OUTER:            ; Outer loop
    NOP                ; Do nothing
    MOVLW Delay2       ; Load Delay2 into WREG
    MOVWF  Counter_L   ; Set Counter_L to Delay2
    
LOOP_INNER:            ; Inner loop
    NOP                ; Do nothing
    DECF  Counter_L, F ; Decrement Counter_L
    BNZ   LOOP_INNER   ; If Counter_L is not zero, repeat inner loop
    
    DECF  Counter_H, F ; Decrement Counter_H  
    BNZ   LOOP_OUTER   ; If Counter_H is not zero, repeat outer loop
    
    RETURN
END


