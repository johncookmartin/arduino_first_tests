#include "timer.h"
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>

void initPrescaler(enum timer_prescaler div){

   TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));

   switch(div){
      case PRESCALE_1:
         TCCR1B |= (1 << CS10);
         break;
      case PRESCALE_8:
         TCCR1B |= (1 << CS11);
         break;
      case PRESCALE_64:
         TCCR1B |= (1 << CS11) | (1 << CS10);
         break;
      case PRESCALE_256:
         TCCR1B |= (1 << CS12);
         break;
      case PRESCALE_1024:
         TCCR1B |= (1 << CS12) | (1 << CS10);
         break;
      default:
         break;
   }
}

void setFrequency(int new_frequency){
   ICR1 = new_frequency;
   OCR1A = new_frequency / 2;
}

void initTimer(uint16_t frequency, enum timer_prescaler divider){
   // Use ICR1 as TOP, Fast PWM mode
   TCCR1A &= ~(1 << WGM10);
   TCCR1A |= (1 << WGM11);
   TCCR1B |= (1 << WGM12);
   TCCR1B |= (1 << WGM13);
   
   initPrescaler(divider);
   setFrequency(frequency);

   // Enable interrupt
   TIMSK1 |= (1 << OCIE1A);

   sei();
}



