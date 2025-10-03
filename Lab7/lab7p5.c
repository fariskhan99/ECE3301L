#include <p18f4620.h>
#include <stdio.h>
#include <math.h>
#include <usart.h>

#pragma config OSC = INTIO67     
#pragma config WDT=OFF           
#pragma config LVP=OFF           
#pragma config BOREN =OFF 

// Strretlight Color Codes
#define OFF     0
#define RED     1
#define GREEN   2
#define YELLOW  3

// North-south & LT Control Ports
#define NS_RED  PORTAbits.RA1 
#define NS_GREEN PORTAbits.RA2 
#define NSLT_RED  PORTCbits.RC2 
#define NSLT_GREEN PORTCbits.RC3 

// East-west & LT Control Ports
#define EW_RED  PORTCbits.RC4 
#define EW_GREEN PORTCbits.RC5 
#define EWLT_RED  PORTBbits.RB7 
#define EWLT_GREEN PORTEbits.RE0 

//Define all switches on dip switch
#define NSPED_SW PORTAbits.RA3
#define EWPED_SW PORTAbits.RA4
#define NSLT_SW PORTAbits.RA5
#define EWLT_SW PORTCbits.RC0

#define SEC_LED PORTEbits.RE2
#define MODE_LED PORTEbits.RE1 

const char LED_7seg[10] = {0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10}; //7-segment array

/*************************************************************/ 
/*                          Prototype Area                   */ 
/*************************************************************/ 
void putch(char c);
void init_UART(void);
void Select_ADC_Channel(char channel);
unsigned int get_full_ADC(void);
void Wait_One_Second(void);
void Wait_Half_Second(void);
void Wait_N_Seconds(char seconds);
void Init_ADC(void);
void Init_TRIS(void);
void Set_NS(char color);
void Set_NSLT(char color);
void Set_EW(char color);
void Set_EWLT(char color);
void Wait_One_Second_With_Beep(void);
void Activate_Buzzer(void);
void Deactivate_Buzzer(void);
void PED_Control(char Direction, char Num_Sec);
void Night_Mode(void);
void Day_Mode(void);



void putch(char c) 
{
    while (!TRMT);   // Wait for transmit register to be empty
    TXREG = c;       // Load the character into the transmit register
}

/*****************************************************************/ 
/*                      Init_UART(): Function to initialize UART */
/*****************************************************************/
void init_UART() 
{
    OpenUSART(USART_TX_INT_OFF & USART_RX_INT_OFF & 
              USART_ASYNCH_MODE & USART_EIGHT_BIT & 
              USART_CONT_RX & USART_BRGH_HIGH, 25);
    OSCCON = 0x70;  // Set oscillator control to 8 MHz
                   
}

/**********************************************************************************/ 
/*                Select_ADC_Channel(char): Select ADC channel for reading voltage */
/**********************************************************************************/
void Select_ADC_Channel(char channel) 
{
    ADCON0 = channel * 4 + 1;   // Set ADC channel and enable ADC
}

/**********************************************************************/ 
/*                     get_full_ADC(void): Read ADC value from channel */
/**********************************************************************/
unsigned int get_full_ADC(void) 
{
    int result;	
    ADCON0bits.GO = 1;           // Start ADC conversion
    while (ADCON0bits.DONE == 1);     // Wait for conversion to complete
    result = (ADRESH * 0x100) + ADRESL;  // Combine high and low bytes
    return result;
}

/*****************************************************************/ 
/*           Wait_One_Second(): Function to delay by a second    */
/*****************************************************************/
void Wait_One_Second() {
    SEC_LED = 1;
    Wait_Half_Second();
    SEC_LED = 0;
    Wait_Half_Second();
}

/*****************************************************************/ 
/*     Wait_Half_Second(): Function to delay by half a second    */
/*****************************************************************/
void Wait_Half_Second() 
{
    T0CON = 0x03;       
    TMR0L = 0x75;       
    TMR0H = 0x0B;       
    INTCONbits.TMR0IF = 0;  // Clear the Timer 0 flag
    T0CONbits.TMR0ON = 1;   // Turn on Timer 0

    while (INTCONbits.TMR0IF == 0);  // Wait for the Timer Flag to be 1
    T0CONbits.TMR0ON = 0;   // Turn off Timer 0
}

/*****************************************************************/ 
/* Wait_N_Second(char): Function to delay by N number of seconds*/
/*****************************************************************/
void Wait_N_Seconds (char seconds) 
{ 
 char I; 
 for (I = 0; I< seconds; I++) 
 { 
  Wait_One_Second(); 
 } 
}

/***************************************************************/ 
/*                      Init_ADC(): Function to initialize ADC */
/***************************************************************/
void Init_ADC() 
{
    ADCON0 = 0x01;   // Select AN0, turn ADC on
    ADCON1 = 0x0E;   // Configure AN0 as analog, others digital
    ADCON2 = 0xA9;   
}

/****************************************************************************/ 
/*                    Init_TRIS(void): Function to initialize TRIS registers*/
/****************************************************************************/
void Init_TRIS(void) 
{
    TRISA = 0xF9;    // Set Port A as input
    TRISB = 0x00;    // Set Port B as output
    TRISC = 0x01;    // Set Port C as output
    TRISD = 0x00;    // Set Port D as output
    TRISE = 0x00;    // Set Port E as output
}

/****************************************************************************/ 
/*              Set_NS(char): Function to set the color for the NS RGB light*/
/****************************************************************************/
void Set_NS(char color) {
    switch (color) {
        case OFF:    NS_RED = 0; NS_GREEN = 0; break;
        case RED:    NS_RED = 1; NS_GREEN = 0; break;
        case GREEN:  NS_RED = 0; NS_GREEN = 1; break;
        case YELLOW: NS_RED = 1; NS_GREEN = 1; break;
    }
}

/****************************************************************************/ 
/*          Set_NSLT(char): Function to set the color for the NSLT RGB light*/
/****************************************************************************/
void Set_NSLT(char color) {
    switch (color) {
        case OFF:    NSLT_RED = 0; NSLT_GREEN = 0; break;
        case RED:    NSLT_RED = 1; NSLT_GREEN = 0; break;
        case GREEN:  NSLT_RED = 0; NSLT_GREEN = 1; break;
        case YELLOW: NSLT_RED = 1; NSLT_GREEN = 1; break;
    }
}

/****************************************************************************/ 
/*          Set_EW(char): Function to set the the color for the EW RGB light*/
/****************************************************************************/
void Set_EW(char color) {
    switch (color) {
        case OFF:    EW_RED = 0; EW_GREEN = 0; break;
        case RED:    EW_RED = 1; EW_GREEN = 0; break;
        case GREEN:  EW_RED = 0; EW_GREEN = 1; break;
        case YELLOW: EW_RED = 1; EW_GREEN = 1; break;
    }
}

/****************************************************************************/ 
/*          Set_EWLT(char): Function to set the color for the EWLT RGB light*/
/****************************************************************************/
void Set_EWLT(char color) {
    switch (color) {
        case OFF:    EWLT_RED = 0; EWLT_GREEN = 0; break;
        case RED:    EWLT_RED = 1; EWLT_GREEN = 0; break;
        case GREEN:  EWLT_RED = 0; EWLT_GREEN = 1; break;
        case YELLOW: EWLT_RED = 1; EWLT_GREEN = 1; break;
    }
}

/******************************************************************************************/ 
/*  Wait_One_Second_With_Beep(): Function that uses delay routines while activating buzzer*/
/******************************************************************************************/
void Wait_One_Second_With_Beep() 
{ 
    SEC_LED = 1;   
    Activate_Buzzer();   
    Wait_Half_Second();   
    SEC_LED = 0;   
    Deactivate_Buzzer ();   
    Wait_Half_Second();   
} 

/*******************************************************/ 
/*  Activate_Buzzer(): Function that enables the buzzer*/
/*******************************************************/
void Activate_Buzzer() 
{ 
    PR2 = 0b11111001 ; 
    T2CON = 0b00000101 ; 
    CCPR2L = 0b01001010 ; 
    CCP2CON = 0b00111100 ; 
} 

/**********************************************************/ 
/*  Deactivate_Buzzer(): Function that disables the buzzer*/
/**********************************************************/
void Deactivate_Buzzer() 
{ 
    CCP2CON = 0x0; 
    PORTCbits.RC1 = 0; 
}

/***************************************************************************************************************/ 
/*  PED_Control(char, char): Function that will control color of lights and if upper or lower digit counts down*/
/***************************************************************************************************************/
void PED_Control(char Direction, char Num_Sec){ //control pedestrian 

    PORTD = 0xFF;   // Turn off upper digit
    PORTB = 0xFF;   // Turn off lower digit 
    
    for(int i = Num_Sec - 1; i >0; i--){
        switch(Direction)
        { //choose direction 
            case 0:
                PORTD = LED_7seg[i]; //display current number with countdown 
                break;
            case 1:
                PORTB = LED_7seg[i]; //display current number with countdown 
                break; 
        }
        Wait_One_Second_With_Beep(); //wait a second with beep 
    }
    PORTD = 0xFF;   // Turn off upper digit
    PORTB = 0xFF;   // Turn off lower digit 
    
    Wait_One_Second_With_Beep(); //wait second with beep 
} 

/************************************************************************/ 
/*  Night_Mode(): Function that follows the routine when MODE_LED is off*/
/************************************************************************/
void Night_Mode()
{
    
    // Turn off 7-Segment Display
    PORTB = 0xFF;
    PORTD = 0xFF;
    
    Set_EW(RED);
    Set_EWLT(RED);
    Set_NSLT(RED);
    Set_NS(GREEN);
    Wait_N_Seconds(6);
    Set_NS(YELLOW);
    Wait_N_Seconds(3);
    Set_NS(RED);
    if (EWLT_SW == 1)
    {
        Set_EWLT(GREEN);
        Wait_N_Seconds(7);
        Set_EWLT(YELLOW);
        Wait_N_Seconds(3);
        Set_EWLT(RED);
    }
    Set_EW(GREEN);
    Wait_N_Seconds(6);
    Set_EW(YELLOW);
    Wait_N_Seconds(3);
    Set_EW(RED);
    if (NSLT_SW == 1)
    {
        Set_NSLT(GREEN);
        Wait_N_Seconds(8);
        Set_NSLT(YELLOW);
        Wait_N_Seconds(3);
        Set_NSLT(RED);
    }  
}

/************************************************************************/ 
/*  Day_Mode(): Function that follows the routine when MODE_LED is on****/
/************************************************************************/
void Day_Mode()
{
    // Turn off 7-Segment Display
    PORTB = 0xFF;
    PORTD = 0xFF;
    
    Set_EW(RED);
    Set_EWLT(RED);
    Set_NSLT(RED);
    Set_NS(GREEN);
    if (NSPED_SW == 1)
    {
        PED_Control(0,8);
    }
    Wait_N_Seconds(7);
    Set_NS(YELLOW);
    Wait_N_Seconds(3);
    Set_NS(RED);
    if (EWLT_SW == 1)
    {
        Set_EWLT(GREEN);
        Wait_N_Seconds(8);
        Set_EWLT(YELLOW);
        Wait_N_Seconds(3);
        Set_EWLT(RED);
    }
    Set_EW(GREEN);
    if(EWPED_SW == 1)
    {
        PED_Control(1,7);
    }
    Set_EW(GREEN);
    Wait_N_Seconds(6);
    Set_EW(YELLOW);
    Wait_N_Seconds(3);
    Set_EW(RED);
    if(NSLT_SW == 1)
    {
        Set_NSLT(GREEN);
        Wait_N_Seconds(7);
        Set_NSLT(YELLOW);
        Wait_N_Seconds(3);
        Set_NSLT(RED);
    }
}

/*************************************************************/ 
/*                           Main Function                   */
/*************************************************************/
void main() 
{
    init_UART();
    Init_ADC();
    Init_TRIS();
    Select_ADC_Channel(0);
    while (1) 
    {
        int num_step = get_full_ADC();
        float volt = num_step * 5 / 1024.0;
        int MODE = volt < 2.5 ? 1:0;
        printf("Voltage Photo Resistor = %f \r\n", volt);
        
        if (MODE)
        {
            MODE_LED = 1;
            Day_Mode();
        }
        else
        {
            MODE_LED = 0;
            Night_Mode();
        } 
    }
}