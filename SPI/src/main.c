/*
Code modified from https://drive.google.com/file/d/1w4-4e3IMD4FH1VEBJ9-vYd_PjK9KZPNM/view
and https://www.youtube.com/watch?v=GFZlZAJersQ
*/

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

// SPI INTERFACE DEFINES
#define MOSI PORTB3  // MOSI it's PORT B, PIN 3
#define MISO PORTB4  // MISO it's PORT B, PIN 4
#define SCK  PORTB5  // SCK it's PORT B, PIN 5
#define SS   PORTB2  // SS it's PORT B, PIN 2

// RESET THE DISPLAY
#define RST PORTB0  // RESET it's PORT B, PIN 0

// DISPLAY MODE SELECT - Input to select either command/address or data input.
#define DC PORTB1  // DC it's PORT B, PIN 1

// codes array of word "HELLO"
static const unsigned char HELLO[40] = {0xFF, 0xFF, 0xFF, 0x18, 0x18, 0x18, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xDB, 0xDB, 0xDB, 0x00, 0xFF, 0xFF, 0xFF,
                                        0xC0, 0xC0, 0xC0, 0x00, 0xFF, 0xFF, 0xFF, 0xC0, 0xC0, 0xC0, 0x00, 0xFF, 0xFF, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xFF, 0xFF};

// codes array of word "SARAH"
static const unsigned char SARAH[50] = {0xDF, 0xDF, 0xDB, 0xDB, 0xDB, 0xDB, 0xFB, 0xFB, 0xFB, 0x00, 0xC0, 0xF0, 0xFC, 0x3F, 0x33, 0x33, 0x3F,
                                        0xFC, 0xF0, 0xC0, 0x00, 0xFF, 0xFF, 0xFF, 0x3B, 0x7B, 0xDF, 0xDF, 0xDF, 0x00, 0xC0, 0xF0, 0xFC, 0x3F,
                                        0x33, 0x33, 0x3F, 0xFC, 0xF0, 0xC0, 0x00, 0xFF, 0xFF, 0xFF, 0x18, 0x18, 0x18, 0xFF, 0xFF, 0xFF};

// Port Initialization
void Port_Init()
{
   DDRB = (1 << MOSI) | (1 << SCK) | (1 << SS) | (1 << RST) | (1 << DC);  // Set MOSI,SCK,SS,RST,DC as output, all others input
   PORTB |= (1 << RST);                                                   // Set RST pin as high
   PORTB |= (1 << SS);                                                    // Set SS pin as high - Display is Disable
}

// SPI Initialization
void SPI_Init()
{
   SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);  // Enable SPI, Set as Master, Set Prescaler as Fosc/16 in SPI control register
}

// Display Enable
void SPI_SS_Enable()
{
   PORTB &= ~(1 << SS);  // Enable the SS pin to logic 0
}

// Display Disable
void SPI_SS_Disable()
{
   PORTB |= (1 << SS);  // Disable the SS pin to logic 1
}

// Function to send data into the display buffer
void SPI_Tranceiver(unsigned char data)
{
   SPDR = data;  // Load data into the buffer
   while (!(SPSR & (1 << SPIF)))
      ;  // Wait until transmission complete
}

// Reset the Display at the beginning of initialization
void Display_Reset()
{
   PORTB &= ~(1 << RST);
   _delay_ms(100);
   PORTB |= (1 << RST);
}

// Command write function
void Display_Cmnd(unsigned char data)
{
   PORTB &= ~(1 << DC);   // make DC pin to logic 0 for command operation
   SPI_Tranceiver(data);  // send data on data register
   PORTB |= (1 << DC);    // make DC pin to logic high for data operation
}

// Initialization of Display
void Display_init()
{
   Display_Reset();     // reset the display
   Display_Cmnd(0x21);  // command set in addition mode
   Display_Cmnd(0xC0);  // set the voltage by sending C0 means VOP = 5V
   Display_Cmnd(0x07);  // set the temp. coefficient to 3
   Display_Cmnd(0x13);  // set value of Voltage Bias System
   Display_Cmnd(0x20);  // command set in basic mode
   Display_Cmnd(0x0C);  // display result in normal mode
}

// Clear the Display
void Display_Clear()
{
   PORTB |= (1 << DC);  // make DC pin to logic high for data operation
   for (int k = 0; k <= 503; k++)
   {
      SPI_Tranceiver(0x00);
   }
   PORTB &= ~(1 << DC);  // make DC pin to logic zero for command operation
}

// set the column and row  to the position of displaying result on LCD Display
void Display_SetXY(unsigned char x, unsigned char y)
{
   Display_Cmnd(0x80 | x);  // column (0-83)
   Display_Cmnd(0x40 | y);  // row (0-5)
}

// Function to display word "HELLO"
void Display_HELLO()
{
   // Word "TEMP"
   Display_SetXY(20, 1);  // Set the address of position on display (up row)
   for (int index = 0; index < sizeof(HELLO) / sizeof(unsigned char); index++)
   {
      SPDR = HELLO[index];  // Load codes array data into the buffer of display
      while (!(SPSR & (1 << SPIF)))
         ;  // Wait until transmission complete
      _delay_ms(10);
   }
}

// Function to display word "SARAH"
void Display_SARAH()
{
   // Word "TEMP"
   Display_SetXY(15, 3);  // Set the address of position on display (up row)
   for (int index = 0; index < sizeof(SARAH) / sizeof(unsigned char); index++)
   {
      SPDR = SARAH[index];  // Load codes array data into the buffer of display
      while (!(SPSR & (1 << SPIF)))
         ;  // Wait until transmission complete
      _delay_ms(10);
   }
}

int main(void)
{
   Port_Init();      // Port Initialization
   SPI_Init();       // SPI Initialization
   SPI_SS_Enable();  // Display Enable
   Display_init();   // Display initialization
   Display_Clear();  // Display clear
   Display_HELLO();  // Display word "HELLO"
   Display_SARAH();  // Display word "SARAH"
}