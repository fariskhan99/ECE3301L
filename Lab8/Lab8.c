#pragma config OSC = INTIO67
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config BOREN = OFF

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <math.h>
#include <p18f4620.h>
#include <string.h>

#include "main.h"
#include "utils.h"
#include "Main_Screen.h"
#include "ST7735_TFT.h"

#define NSPED_SW PORTAbits.RA3
#define EWPED_SW PORTAbits.RA4
#define NSLT_SW PORTAbits.RA5
#define EWLT_SW PORTCbits.RC0

#define SEC_LED PORTEbits.RE2
#define MODE_LED PORTEbits.RE1 

void Set_NS(char color);
void Set_NS_LT(char color);
void Set_EW(char color);
void Set_EW_LT(char color);
void PED_Control( char Direction, char Num_Sec);
void Day_Mode();
void Night_Mode();
void Wait_N_Seconds (char);
void Wait_One_Second_With_Beep();
void Wait_One_Second();


#define _XTAL_FREQ  8000000             // Set operation for 8 Mhz
#define TMR_CLOCK   _XTAL_FREQ/4        // Timer Clock 2 Mhz

char dir;
char Count;                             // RAM variable for Second Count
char PED_Count;                         // RAM variable for Second Pedestrian Count

char  MODE;
char direction;
float volt;

extern char Light_Sensor;

/*************************************************************/ 
/*               main(): Main function                */ 
/*************************************************************/ 
void main(void)
{
    OSCCON = 0x70;                      // set the system clock to be 1MHz 1/4 of the 4MHz
    TRISA = 0xF9;                       //inputs RA0/AN0-RA4 inputs,RA5 output
    TRISB = 0x07;                       // outputs
    TRISC = 0x01;                       // set PORTC as outputs
    TRISD = 0x00;                       // set PORTD as outputs
    TRISE = 0x00;                       // set PORTE as outputs
    Init_ADC();
    Init_UART();

    Initialize_LCD_Screen();                        // Initialize the TFT screen

//    while (1)
//    {
//        Rcmd2red();
//        delay_ms(2);
//        
//    }

    volt = Read_Volt(0);                        // 

    Light_Sensor = volt < 2.5 ? 1:0;                 // Mode = 1, Day_mode, Mode = 0 Night_mode

    while(1)                                    // forever loop
    {

        if (Light_Sensor == 1)    
        {
            MODE = 1;
            Day_Mode();                         // calls Day_Mode() function
            update_LCD_misc();
        }
        else
        {
            MODE = 0;
            Night_Mode();                       // calls Night_Mode() function
            update_LCD_misc();
        }
    } 
}

/*************************************************************/ 
/*               Set_NS(char): Control NS lights             */ 
/*************************************************************/ 
void Set_NS(char color)
{
    direction = NS; 
    update_LCD_color (direction, color);
    switch (color) 
    {
        case OFF:    NS_RED = 0; NS_GREEN = 0; break;
        case RED:    NS_RED = 1; NS_GREEN = 0; break;
        case GREEN:  NS_RED = 0; NS_GREEN = 1; break;
        case YELLOW: NS_RED = 1; NS_GREEN = 1; break;
    } // add code here
}

/*************************************************************/ 
/*            Set_NS_LT(char): Control NS Left-Turn lights   */ 
/*************************************************************/
void Set_NS_LT(char color)
{
    direction = NSLT; 
    update_LCD_color (direction, color);
    switch (color) 
    {
        case OFF:    NSLT_RED = 0; NSLT_GREEN = 0; break;
        case RED:    NSLT_RED = 1; NSLT_GREEN = 0; break;
        case GREEN:  NSLT_RED = 0; NSLT_GREEN = 1; break;
        case YELLOW: NSLT_RED = 1; NSLT_GREEN = 1; break;
    }
}

/*************************************************************/ 
/*               Set_EW(char): Control EW lights             */ 
/*************************************************************/ 
void Set_EW(char color)
{
    direction = EW; 
    update_LCD_color (direction, color);
    switch (color) 
    {
        case OFF:    EW_RED = 0; EW_GREEN = 0; break;
        case RED:    EW_RED = 1; EW_GREEN = 0; break;
        case GREEN:  EW_RED = 0; EW_GREEN = 1; break;
        case YELLOW: EW_RED = 1; EW_GREEN = 1; break;
    }
}

/*************************************************************/ 
/*           Set_EW_LT(char): Control EW Left-Turn lights    */ 
/*************************************************************/ 
void Set_EW_LT(char color)
{
    direction = EWLT; 
    update_LCD_color (direction, color);
    switch (color) 
    {
        case OFF:    EWLT_RED = 0; EWLT_GREEN = 0; break;
        case RED:    EWLT_RED = 1; EWLT_GREEN = 0; break;
        case GREEN:  EWLT_RED = 0; EWLT_GREEN = 1; break;
        case YELLOW: EWLT_RED = 1; EWLT_GREEN = 1; break;
    }
}

/*************************************************************/ 
/*      PED_Control(char, char): Pedestrian countdown timer  */ 
/*************************************************************/ 
void PED_Control(char direction, char Num_Sec)
{
    // Turn off the upper and lower digits on the 7-segment display
    

    // Countdown using the update_LCD_PED_Count routine
    for (int i = Num_Sec - 1; i >= 0; i--)
    {
        
        update_LCD_PED_Count(direction, i);
        Wait_One_Second_With_Beep();
    }
    
   // Wait_One_Second_With_Beep();
}

/*************************************************************/ 
/*               Day_Mode(): Handles day traffic logic       */ 
/*************************************************************/ 
void Day_Mode()
{
    Set_EW(RED);
    Set_EW_LT(RED);
    Set_NS_LT(RED);
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
        Set_EW_LT(GREEN);
        Wait_N_Seconds(8);
        Set_EW_LT(YELLOW);
        Wait_N_Seconds(3);
        Set_EW_LT(RED);
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
        Set_NS_LT(GREEN);
        Wait_N_Seconds(7);
        Set_NS_LT(YELLOW);
        Wait_N_Seconds(3);
        Set_NS_LT(RED);
    }
}

/*************************************************************/ 
/*              Night_Mode(): Handles night logic            */ 
/*************************************************************/
void Night_Mode()
{
    Set_EW(RED);
    Set_EW_LT(RED);
    Set_NS_LT(RED);
    Set_NS(GREEN);
    Wait_N_Seconds(6);
    Set_NS(YELLOW);
    Wait_N_Seconds(3);
    Set_NS(RED);
    if (EWLT_SW == 1)
    {
        Set_EW_LT(GREEN);
        Wait_N_Seconds(7);
        Set_EW_LT(YELLOW);
        Wait_N_Seconds(3);
        Set_EW_LT(RED);
    }
    Set_EW(GREEN);
    Wait_N_Seconds(6);
    Set_EW(YELLOW);
    Wait_N_Seconds(3);
    Set_EW(RED);
    if (NSLT_SW == 1)
    {
        Set_NS_LT(GREEN);
        Wait_N_Seconds(8);
        Set_NS_LT(YELLOW);
        Wait_N_Seconds(3);
        Set_NS_LT(RED);
    }  
}

/***********************************************/
/*      Wait_One_Second(): Delay with visual   */ 
/***********************************************/ 
void Wait_One_Second()							//creates one second delay and blinking asterisk
{
    SEC_LED = 1;
    Draw_Star();
    Wait_Half_Second();                         // Wait for half second (or 500 msec)
    SEC_LED = 0;
    Erase_Star();
    Wait_Half_Second();                         // Wait for half second (or 500 msec)
	update_LCD_misc();
}

/*************************************************************/ 
/*   Wait_One_Second_With_Beep(): Delay with sound and LED   */ 
/*************************************************************/ 
void Wait_One_Second_With_Beep()				//creates one second delay as well as sound buzzer
{
    SEC_LED = 1;
    Draw_Star();
    Activate_Buzzer();
    Wait_Half_Second();                         // Wait for half second (or 500 msec)

    SEC_LED = 0;
    Erase_Star();
    Deactivate_Buzzer();
    Wait_Half_Second();                         // Wait for half second (or 500 msec)
	update_LCD_misc();
}
 
/*************************************************************/ 
/*      Wait_N_Seconds(char): Delays for N seconds           */ 
/*************************************************************/ 
void Wait_N_Seconds (char seconds)
{
    char I;
    for (I = seconds; I> 0; I--)
    {
		update_LCD_count(direction, I);
        Wait_One_Second();          			// calls Wait_One_Second for x number of times
    }
	update_LCD_count(direction, 0);
}
  







