#include "../common/analog.h"
#include "../common/serial.h"
#include "../common/timer.h"
#include "../common/sound.h"
#include <avr/io.h>
#include <stdlib.h>
#include <stdbool.h>
#include <util/delay.h>

/*
Romeo            Trainer              Pololu              Sharp
ADC0                                                      Vo
ADC1                                  1
ADC2                                  2
ADC3                                  3
ADC4                                  4
ADC5                                  5
ADC6                                  6
ADC7                                  7
PD2              D1A
PD3              D1B
PD4              D1C
PD5              D1D
PD6              D1E
PD7              D1F
PB0              D1G
PB1              D1H
PB2              BZ1
PB3              DS1
PB4              S2
PB6              BZ1
*/

bool USE_8_BIT = false;
unsigned int BAUD = 9600;
int NUMBER_OF_SENSORS = 7;
uint16_t POLOLU_VALUES[7];

bool check_mode(bool prev){
   bool result = prev;
   if (!(PINB & (1 << PB4))) {
      _delay_ms(100);
      if (!(PINB & (1 << PB4))){
         result = !prev;
      }
   }
   return result;
}

/*
   B Major Scale:
   2000 = B3
   1780 = C#4  
   1587 = D#4
   1498 = E4
   1335 = F#4
   1189 = G#4
   1059 = A#4
   1000 = B4
*/

void sound_test(){
   tone(2000); //b
   _delay_ms(500);
   tone(1780); //c#
   _delay_ms(500);
   tone(1587); //d#
   _delay_ms(500);
   tone(1498); //e
   _delay_ms(500);
   tone(1335); //f#
   _delay_ms(500);
   tone(1189); //g#
   _delay_ms(500);
   tone(1059); //a#
   _delay_ms(500);
   tone(1000); //b
   _delay_ms(500);
   tone(1059); //a#
   _delay_ms(500);
   tone(1189); //g#
   _delay_ms(500);
   tone(1335); //f#
   _delay_ms(500);
   tone(1498); //e
   _delay_ms(500);
   tone(1587); //d#
   _delay_ms(500);
   tone(1780); //c#
   _delay_ms(500);
   tone(2000); //b
   _delay_ms(500);
}

uint16_t calculate_postion(const uint16_t *arr, int size){
   uint32_t numerator = 0;
   uint32_t denominator = 0;
   for (int i = 0; i < size; i++){
      uint32_t coeff = (i+1) * 1000;
      numerator += coeff * (1023 - arr[i]);
      denominator += (1023- arr[i]);
   }
   if (denominator == 0){
      return 0;
   } else {
      return numerator/denominator;
   }
}

// ADC1   1
// ADC3   2
// ADC4   3
// ADC5   4
// ADC6   5
uint16_t read_pololu_tone(uint16_t current_position){
   for (int i = 1; i <= NUMBER_OF_SENSORS; i++){
      int pin = i;
      uint16_t raw_value = analog(pin, USE_8_BIT);
      uint16_t new_value = ((raw_value + 50) / 100) * 100;
      if (abs(POLOLU_VALUES[i-1] - new_value)){

            POLOLU_VALUES[i-1] = new_value;
         }
   }

   uint16_t new_position = calculate_postion(POLOLU_VALUES, 7);
   if (abs(new_position - current_position) > 100){
      return new_position;
   }
   return current_position;
}

uint16_t convert_pololu_tone(uint16_t current_position){
   PORTD &= ~(0b11111100);
   PORTB &= ~(0b00000011);
   if (current_position < 1000){
      PORTD |= (1 << PD2);
      return 2000;
   } else if(current_position < 2000){
      PORTD |= (1 << PD3);
      return 1780;
   } else if (current_position < 3000){
      PORTD |= (1 << PD4);
      return 1587;
   } else if (current_position < 4000) {
      PORTD |= (1 << PD5);
      return 1498;
   } else if (current_position < 5000) {
      PORTD |= (1 << PD6);
      return 1335;
   } else if (current_position < 6000){
      PORTD |= (1 << PD7);
      return 1189;
   } else if (current_position < 7000){
      PORTB |= (1 << PB0);
      return 1059;
   } else {
      PORTB |= (1 << PB1);
      return 1000;
   }
}

int main(void) {
   // Initialize
   DDRD = 0b11111100;
   DDRB = 0b00100111;
   DDRB |= (1 << PB5);
   DDRB &= ~(1 << PB4);
   DDRB &= ~(1 << PB3);
   PORTB |= (1 << PB4);
   PORTB |= (1 << PB3);

   initUART(BAUD);
   initADC(USE_8_BIT);
   initTimer(2000, PRESCALE_8);
   initSound(&PORTB, &DDRB, PB2);

   transmitString("Welcome to Theremin 1!");
   bool enable_sound = false;
   uint16_t current_pololu_position = 0;

   sound_test();

   while(1){
      enable_sound = check_mode(enable_sound);
      if (enable_sound | !(PINB & (1 << PB3))){
         PORTB |= (1 << PB5);
         current_pololu_position = read_pololu_tone(current_pololu_position);
         tone(convert_pololu_tone(current_pololu_position));
      }
      else {
         mute();
         PORTD &= ~(0b11111100);
         PORTB &= ~(0b00100011);
      }
   }
}
