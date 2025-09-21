#include "timer.h"
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>

static uint16_t count_value;

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

void setFrequency(int frequency){
   // Check division by zero
   if (frequency > 0) {
      OCR1A = count_value / frequency;
   } else {
      // Default to 50% duty cycle
      OCR1A = count_value / 2;
   }
}

void initTimer(uint16_t count, int frequency, enum timer_prescaler divider){
   count_value = count;
   
   // Set ICR1 as TOP value
   ICR1 = count_value;

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



