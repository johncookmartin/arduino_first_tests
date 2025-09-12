#include "../common/analog.h"
#include "../common/serial.h"
#include <avr/io.h>
#include <stdlib.h>
#include <stdbool.h>
#include <util/delay.h>

uint16_t calculate_position(const uint16_t *arr, int size){
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

int main(void) {
   // Initialize
   bool use_8_bit = false;
   initUART(9600);
   initADC(use_8_bit);

   transmitString("Welcome to Pololu Reader!");
   int number_of_sensors = 7;
   uint16_t current_values[number_of_sensors];
   uint16_t current_position = 0;

   while(1){
      for (int i = 0; i < number_of_sensors; i++){
         int pin = i;
         if (i == 6){ //missing pin 6
            pin = 7;
         }
         uint16_t raw_value = analog(pin, use_8_bit);
         uint16_t new_value = ((raw_value + 50) / 100) * 100;
         if (abs(current_values[i] - new_value)){

            current_values[i] = new_value;
         }
      }
      
      uint16_t new_position = calculate_position(current_values, number_of_sensors);
      if (abs(new_position - current_position) > 5){
         printDec(new_position);
         current_position = new_position;
      }
      _delay_ms(50);
   }
}
