#include "analog.h"
#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

void initADC(bool use_8_bit){
   ADCSRA |= (1 << ADEN);
   //enable ADC
   ADMUX |= (1 << REFS0);
   //Vref connect internally to AVcc
   if (use_8_bit){
      ADMUX |= (1 << ADLAR);
      //left adjusted for 8-bit precision
   }   
   ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
   //Set prescale to 8 (16000000/128 = 125Khz)
   ADCSRA |= (1 << ADSC);
   // start conversion 1st time will take 25 cycles
}

uint16_t analog(uint8_t channel, bool is_left){
   ADMUX &= 0xF0; //Clearing the last 4 bits of ADMUX
   ADMUX |= channel; //Selecting channel
   ADCSRA |= (1 << ADSC); //Start conversion process
   //wait for complete conversion and return the result
   while (ADCSRA & (1 << ADSC));

   if (is_left){
      return ADCH;
   } else {
      return ADC;
   }
}