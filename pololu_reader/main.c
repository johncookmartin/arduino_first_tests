#include "../common/analog.h"
#include "../common/serial.h"
#include <avr/io.h>
#include <stdlib.h>
#include <stdbool.h>

uint16_t calculate_position(const uint16_t *arr, int size){
   uint16_t numerator = 0;
   uint16_t denominator = 0;
   for (int i = 0; i < size; i++){
      uint16_t coeff = (i+1) * 1000;
      numerator += coeff * arr[i];
      denominator += arr[i];
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
   uint16_t current_values[7];
   uint16_t current_position = 0;

   while(1){
      for (int i = 0; i < 7; i++){
         uint16_t new_value = analog(i, use_8_bit);
         if (abs(current_values[i] - new_value) > 5){
            current_values[i] = new_value;
         }
      }
      
      uint16_t new_position = calculate_position(current_values, sizeof(current_values));
      if (abs(new_position - current_position) > 5){
         printDec(new_position);
         current_position = new_position;
      }
   }
}
