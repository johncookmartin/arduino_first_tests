#include "../common/serial.h"
#include <avr/io.h>

int main(void) {
   // Initialize
   initUART(9600);

   transmitString("Hello World!");

   while(1) {
      unsigned char letter = receiveByte();
      if (letter >= 122){
         letter = 'A';
      }
      else {
         letter += 1;
      }

      transmitByte(letter);
      transmitByte('\r');
      transmitByte('\n');
   }
}