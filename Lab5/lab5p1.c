#include <p18f4620.h>
#include <stdio.h>
#include <math.h>
#include <usart.h>

#pragma config OSC = INTIO67
#pragma config WDT=OFF
#pragma config LVP=OFF
#pragma config BOREN =OFF

#define delay 17000

#define D1_RED    PORTCbits.RC0
#define D1_GREEN  PORTCbits.RC1
#define D1_BLUE   PORTCbits.RC2

#define D2_RED    PORTCbits.RC3
#define D2_GREEN  PORTCbits.RC4
#define D2_BLUE   PORTCbits.RC5

#define D3_RED    PORTEbits.RE0
#define D3_GREEN  PORTEbits.RE1
#define D3_BLUE   PORTEbits.RE2

const char LED_7seg[10] = {0x01, 0x4F, 0x12, 0x06, 0x4C, 0x24, 0x20, 0x0F, 0x00, 0x04};

void putch(char c);
void Init_UART(void);
void Init_ADC(void);
void Init_TRIS(void);
void Select_ADC_Channel(char channel);
unsigned int get_full_ADC(void);
void Display_7Seg(int tempF);
void set_D1_color(int temp_F);
void set_D2_color(int temp_F);
void set_D3_color(float voltage);

void SET_D1_RED(void);
void SET_D1_GREEN(void);
void SET_D1_BLUE(void);
void SET_D1_YELLOW(void);
void SET_D1_CYAN(void);
void SET_D1_PURPLE(void);
void SET_D1_WHITE(void);
void SET_D1_OFF(void);

void SET_D2_RED(void);
void SET_D2_GREEN(void);
void SET_D2_BLUE(void);
void SET_D2_YELLOW(void);
void SET_D2_CYAN(void);
void SET_D2_PURPLE(void);
void SET_D2_WHITE(void);
void SET_D2_OFF(void);

void SET_D3_RED(void);
void SET_D3_GREEN(void);
void SET_D3_BLUE(void);
void SET_D3_YELLOW(void);
void SET_D3_CYAN(void);
void SET_D3_PURPLE(void);
void SET_D3_WHITE(void);
void SET_D3_OFF(void);

void Delay_One_Sec(void);


void putch(char c) {
    while (!TRMT);
    TXREG = c;
}

void Init_UART() {
    OpenUSART(USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE &
              USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_HIGH, 25);
    OSCCON = 0x60;
}

void Init_ADC() {
    ADCON0 = 0x01; // Select AN0, turn ADC on
    ADCON1 = 0x0B; // Configure AN0-AN3 as analog, others digital
    ADCON2 = 0xA9; // Right justified
}

void Init_TRIS(void) 
{
    TRISA = 0x0F; // Sets Port A to input 
    TRISB = 0x00; // Sets Port B to output 
    TRISC = 0x00; // Sets Port C to output 
    TRISD = 0x00; // Sets Port D to output 
    TRISE = 0x00; // Sets Port E to output 
}

void Select_ADC_Channel(char channel) // CONNECT APPROPRIATE ADC CHANNEL 
{
    ADCON0 = channel * 4 + 1;
}

unsigned int get_full_ADC(void) 
{
    int result;	
    ADCON0bits.GO = 1;
    while (ADCON0bits.DONE==1);
    result = (ADRESH * 0x100) + ADRESL;
    return result;
}

void Display_7Seg(int temp_F) {
    int upper = temp_F / 10;
    int lower = temp_F % 10;
    PORTD = LED_7seg[upper];
    PORTB = LED_7seg[lower];
}

void set_D1_color(int temp_F) {
    temp_F = temp_F / 10; // grabs the temperature and gets the tens digit
    if (temp_F >= 0 && temp_F < 7) //puts it in the range from 0 to 6
        PORTC = temp_F;            //output the corresponding color
    else
        PORTC = 0x07;             //if 7 or above (70+) display white
}

void set_D2_color(int temp_F) {
    if (temp_F < 32) {
        SET_D2_OFF();    // Below 32°F - Turn off LED
    } else if (temp_F >= 32 && temp_F < 42) {
        SET_D2_RED();    // 32°F - 42°F - Red
    } else if (temp_F >= 42 && temp_F < 52) {
        SET_D2_GREEN();  // 42°F - 52°F - Green
    } else if (temp_F >= 52 && temp_F < 62) {
        SET_D2_YELLOW(); // 52°F - 62°F - Yellow
    } else if (temp_F >= 62 && temp_F < 72) {
        SET_D2_BLUE();   // 62°F - 72°F - Blue
    } else if (temp_F >= 72 && temp_F < 77) {
        SET_D2_PURPLE(); // 72°F - 77°F - Purple
    } else if (temp_F >= 77 && temp_F < 80) {
        SET_D2_CYAN();   // 77°F - 80°F - Cyan
    } else {
        SET_D2_WHITE();  // Above 80°F - White
    }
}



// Set D3 RGB LED color based on light sensor voltage
void set_D3_color(float voltage) {
    if (voltage < 2.6) {
        SET_D3_WHITE();  
    } else if (voltage >= 2.6 && voltage < 2.9) {
        SET_D3_BLUE();  
    } else if (voltage >= 2.9 && voltage < 3.2) {
        SET_D3_GREEN();
    } else if (voltage >= 3.2 && voltage < 3.6) {
        SET_D3_RED();
    } else {
        SET_D3_OFF();   
    }
}

void SET_D1_RED()    { D1_RED = 1; D1_GREEN = 0; D1_BLUE = 0; }
void SET_D1_GREEN()  { D1_RED = 0; D1_GREEN = 1; D1_BLUE = 0; }
void SET_D1_BLUE()   { D1_RED = 0; D1_GREEN = 0; D1_BLUE = 1; }
void SET_D1_YELLOW() { D1_RED = 1; D1_GREEN = 1; D1_BLUE = 0; }
void SET_D1_CYAN()   { D1_RED = 0; D1_GREEN = 1; D1_BLUE = 1; }
void SET_D1_PURPLE() { D1_RED = 1; D1_GREEN = 0; D1_BLUE = 1; }
void SET_D1_WHITE()  { D1_RED = 1; D1_GREEN = 1; D1_BLUE = 1; }
void SET_D1_OFF()    { D1_RED = 0; D1_GREEN = 0; D1_BLUE = 0; }

void SET_D2_RED()    { D2_RED = 1; D2_GREEN = 0; D2_BLUE = 0; }
void SET_D2_GREEN()  { D2_RED = 0; D2_GREEN = 1; D2_BLUE = 0; }
void SET_D2_BLUE()   { D2_RED = 0; D2_GREEN = 0; D2_BLUE = 1; }
void SET_D2_YELLOW() { D2_RED = 1; D2_GREEN = 1; D2_BLUE = 0; }
void SET_D2_CYAN()   { D2_RED = 0; D2_GREEN = 1; D2_BLUE = 1; }
void SET_D2_PURPLE() { D2_RED = 1; D2_GREEN = 0; D2_BLUE = 1; }
void SET_D2_WHITE()  { D2_RED = 1; D2_GREEN = 1; D2_BLUE = 1; }
void SET_D2_OFF()    { D2_RED = 0; D2_GREEN = 0; D2_BLUE = 0; }

void SET_D3_RED()    { D3_RED = 1; D3_GREEN = 0; D3_BLUE = 0; }
void SET_D3_GREEN()  { D3_RED = 0; D3_GREEN = 1; D3_BLUE = 0; }
void SET_D3_BLUE()   { D3_RED = 0; D3_GREEN = 0; D3_BLUE = 1; }
void SET_D3_YELLOW() { D3_RED = 1; D3_GREEN = 1; D3_BLUE = 0; }
void SET_D3_CYAN()   { D3_RED = 0; D3_GREEN = 1; D3_BLUE = 1; }
void SET_D3_PURPLE() { D3_RED = 1; D3_GREEN = 0; D3_BLUE = 1; }
void SET_D3_WHITE()  { D3_RED = 1; D3_GREEN = 1; D3_BLUE = 1; }
void SET_D3_OFF()    { D3_RED = 0; D3_GREEN = 0; D3_BLUE = 0; }

void Delay_One_Sec() {
    for (int i = 0; i < delay; i++); // delay routine
}

void main() {
    Init_UART();
    Init_ADC();
    Init_TRIS(); 

    while (1) 
    {
        Select_ADC_Channel(0);
        int num_step = get_full_ADC();
        float voltage = num_step * 5000.0 / 1024;
        float tempC = (1035.0 - voltage) / 5.50;
        float tempF = (1.80 * tempC) + 32.0;
        int temperature_F = (int) tempF;
        printf("Voltage: %.2f V\r\n", voltage / 1000);
        printf("Temperature: %d F\r\n", temperature_F);

        Display_7Seg(temperature_F);
        set_D1_color(temperature_F);
        set_D2_color(temperature_F);

        Select_ADC_Channel(1);
        num_step = get_full_ADC();
        float light_voltage = num_step * 5000.0 / 1024 /1000;
        printf("Light Voltage: %.2f V\r\n\n", light_voltage);
        set_D3_color(light_voltage);

        Delay_One_Sec();
        
    }
}
