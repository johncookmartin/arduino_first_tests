#include "../common/analog.h"
#include "../common/serial.h"
#include "../common/timer.h"
#include <avr/io.h>




int main(void) {
   // Initialize
   bool use_8_bit = false;
   initUART(9600);
   initADC(use_8_bit);
   initTimer();

   transmitString("Welcome to Theremin 1!");

   while(1){
   }
}
