#include <avr/interrupt.h>
#include <avr/io.h>

#define DELAY_INTERRUPTS 5

volatile uint8_t flag = 0;
static uint8_t pwmDelay = DELAY_INTERRUPTS;

ISR(TIMER2_COMPA_vect) { flag = 1; }

void setupTimer2ForCTC()
{
   TCCR2A = _BV(WGM21);            /* = 0b00000010, TC2 mode 2, CTC - clear timer on match A */
   TCCR2B = _BV(CS22) | _BV(CS20); /* = 0b00000101, clock by F_CPU / 1024 */
   OCR2A = 255;                    /* = 155 */
   TIMSK2 = _BV(OCIE2A);           /* = 0b00000010, enable OCIE2A, match A interrupt */
}

void setupTimer0ForPWM()
{
   TCCR0A = (_BV(WGM00) | _BV(WGM01) | _BV(COM0A1) | _BV(COM0B1)); /* = 0b10100011, TC0 Mode 3, Fast PWM */
   TCCR0B = _BV(CS00);                                             /* TC0 clocked by F_CPU, no prescale */
   OCR0A = 0;                                                      /* start with duty cycle = 0% */
   OCR0B = 255;                                                    /* start with duty cycle = 100% */
}

int main(void)
{
   DDRD |= (_BV(PORTD6) | _BV(PORTD5));
   PORTD &= (~_BV(PORTD6) & ~_BV(PORTD5));

   setupTimer2ForCTC();
   setupTimer0ForPWM();

   sei();

   while (1)
   {
      if (flag == 1)
      {
         flag = 0;
         if (--pwmDelay == 0)
         {
            pwmDelay = DELAY_INTERRUPTS;
            ++OCR0A;
            --OCR0B;
         }
      }
   }
}