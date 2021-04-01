#include <avr/io.h>
#include <avr/interrupt.h>

#define DELAY_INTERRUPTS 100

volatile uint8_t flag = 0;
static uint8_t ledDelay = DELAY_INTERRUPTS;

ISR(TIMER0_COMPA_vect)
{
	flag = 1;
}

//Need an extra layer here so we can abstract away from the hardware registers
//for testing
void setupTimer()
{
	DDRB =  0b00100001;
	PORTB = 0b00000000;

	TCCR0A = 0b00000010;
	TCCR0B = 0b00000101;
	OCR0A = 155; /* (F_CPU / 1024 / 100) - 1 */
	TIMSK0 = 0b00000010;

	sei();	
}

//Need an extra layer here so we can abstract away from the hardware registers
//for testing
void toggleLED()
{
	PORTB ^= 0b00100001;
}

//Can test easily since its not harware specific
void resetFlag()
{
	flag = 0;	
}

//Can test easily since its not harware specific
void resetDelay()
{
	ledDelay = DELAY_INTERRUPTS;
}

//Black box test with expect calls 
//or white box test that whatever happens inside the functions is happening
void resetAndCheckForToggleDelay()
{
	resetFlag();
	if(ledDelay-- == 0)
	{
		resetDelay();
		toggleLED();
	}	
}

void executeLoop()
{
	if(flag == 1)
	{
		resetAndCheckForToggleDelay();
	}
}

int main(void)
{
	setupTimer();

	while(1)
	{
		executeLoop();
	}
}
