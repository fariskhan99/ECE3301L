This lab expands on Assembly programming for the PIC18F4620, introducing arithmetic, logic, branching, and subroutine usage. Implement basic operations, test flags, and use subroutines selected by input switches to control LEDs.  

✅ Lab Tasks  

1’s Complement (Part A) – Read a 4-bit number from DIP switches (PORTA), compute its 1’s complement, and display on LEDs (PORTC).  

Zero Flag Test (Part B) – Complement result checked with the Z flag; conditionally set/clear PORTD bit 1.  

Arithmetic & Logic Operations (Parts C–F)  

  -Addition – Input from PORTA + PORTB, result on PORTC with overflow shown on bit 4.  

  -AND, OR, XOR – Replace addition with corresponding logical operations.  

BCD Conversion (Part G) – Convert 4-bit input into BCD using comparison and conditional addition.  

Subroutines & Function Selection (Part H)  

  -Group each operation into subroutines (SUB_COMP, SUB_ADD, SUB_AND, SUB_OR, SUB_XOR, SUB_BCD).  

  -Use switches on PORTE bits [2:0] to select which routine to execute.  

  -RGB LED (PORTD bits 4–6) indicates the active operation.  

🔑 Key Skills

Arithmetic & logical Assembly instructions (ADDWF, ANDWF, IORWF, XORWF, COMF).  

Branching with flags (BZ, BTFSC, CPFSGT).  

Subroutine creation and CALL/RETURN usage.  

Input/output handling with DIP switches, LEDs, and RGB LEDs.  

Function selection through switch-based decoding logic.  
