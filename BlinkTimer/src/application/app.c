#include <avr/io.h>
#include <avr/interrupt.h>

#define DELAY_INTERRUPTS 100

volatile uint8_t flag = 0;
static uint8_t ledDelay = DELAY_INTERRUPTS;

ISR(TIMER0_COMPA_vect)
{
	flag = 1;
}

int main(void)
{
	DDRB =  0b00100001;
	PORTB = 0b00000000;

	TCCR0A = 0b00000010;
	TCCR0B = 0b00000101;
	OCR0A = 155; /* (F_CPU / 1024 / 100) - 1 */
	TIMSK0 = 0b00000010;

	sei();

	while(1)
	{
		if(flag == 1)
		{
			flag = 0;
			if(ledDelay-- == 0)
			{
				ledDelay = DELAY_INTERRUPTS;
				PORTB ^= 0b00100001;
			}
		}
	}
}
