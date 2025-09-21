#include "../common/analog.h"
#include "../common/serial.h"
#include "../common/timer.h"
#include "../common/sound.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/*
Romeo            Trainer              Pololu              Sharp
ADC0                                                      Vo
ADC1             JP4-1
PC2              JP4-2
ADC3                                  1
ADC4                                  2
ADC5                                  3
ADC6                                  4
*/

int main(void) {
   // Initialize
   bool use_8_bit = false;
   initUART(9600);
   initADC(use_8_bit);
   initTimer(2000, 2, PRESCALE_8);
   initSound(&PORTC, &DDRC, PC2);

   transmitString("Welcome to Theremin 1!");

   while(1){
      tone(2);
      _delay_ms(2000);
      mute();
      _delay_ms(2000);
      tone(1);
      _delay_ms(2000);
      mute();
      _delay_ms(2000);
      tone(2);
      _delay_ms(2000);
      mute();
      _delay_ms(2000);
   }
}
