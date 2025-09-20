#include "timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void initTimer(){
   // Set ICR1 as TOP value
   ICR1 = 2000;

   // Use ICR1 as TOP, Fast PWM mode
   TCCR1A &= ~(1 << WGM10);
   TCCR1A |= (1 << WGM11);
   TCCR1B |= (1 << WGM12);
   TCCR1B |= (1 << WGM13);
   
   //1/8 prescale
   TCCR1B |= (1 << CS11);
   
   // set compare value
   OCR1A = 1000;

   // Enable interrupt
   TIMSK1 |= (1 << OCIE1A);

   sei();
}