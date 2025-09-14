#include "../common/analog.h"
#include "../common/serial.h"
#include <avr/io.h>
#include <stdlib.h>
#include <stdbool.h>
#include <util/delay.h>

uint16_t get_raw_average(const uint16_t *arr, int size){
   uint32_t total = 0;
   for (int i = 0; i < size; i++){
      total += arr[i];
   }
   return total / size;
}

int volt_to_distance[35] = {0, 0, 0, 40, 30, 25, 20, 18, 16, 14, 12, 11, 10, 10, 9, 8, 8, 7, 7, 7, 6, 6, 5, 5, 5, 5, 4, 4, 4, 3, 3, 3, 3, 3, 3};


int main(void) {
   // Initialize
   bool use_8_bit = false;
   initUART(9600);
   initADC(use_8_bit);

   transmitString("Welcome to Sharp Sensor Reader!");
   int sample_size = 5;
   uint16_t current_v = 0;

   while(1){
      uint16_t raw_arr[10];
      for(int i = 0; i < sample_size; i++){
         raw_arr[i] = analog(0, use_8_bit);
      }
      uint16_t raw_adc = get_raw_average(raw_arr, sample_size);
      uint16_t raw_v = (raw_adc * 5) / 100;
      printDec(volt_to_distance[raw_v]);
      _delay_ms(1000);
   }
}
