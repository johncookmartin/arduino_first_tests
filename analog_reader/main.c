#include "../common/serial.h"
#include "../common/analog.h"
#include <avr/io.h>
#include <stdlib.h>
#include <stdbool.h>

int main(void) {
   // Initialize
   bool use_8_bit = true;
   initUART(9600);
   initADC(use_8_bit);

   transmitString("Welcome to Analog Reader!");
   uint16_t current_value = 0;

   while(1){
      uint16_t new_value = analog(0, use_8_bit);
      if (abs(current_value - new_value) > 5){
         printDec(new_value);
         current_value = new_value;
      } 
   }
}
