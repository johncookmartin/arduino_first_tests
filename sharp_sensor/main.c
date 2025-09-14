#include "../common/analog.h"
#include "../common/serial.h"
#include <avr/io.h>
#include <stdlib.h>
#include <stdbool.h>
#include <util/delay.h>

uint16_t calculate_distance(uint16_t raw_v){

}

int main(void) {
   // Initialize
   bool use_8_bit = false;
   initUART(9600);
   initADC(use_8_bit);

   transmitString("Welcome to Sharp Sensor Reader!");
   uint16_t current_v = 0;

   while(1){
      uint16_t raw_v = analog(0, use_8_bit);
      printDec(raw_v);
      _delay_ms(50);
   }
}
