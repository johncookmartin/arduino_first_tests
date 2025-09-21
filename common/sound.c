#include "sound.h"
#include "timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

static volatile bool tone_enabled = false;
static volatile uint8_t *ddr;
static volatile uint8_t *port;
static uint8_t pin = 0;


void initSound(volatile uint8_t *input_port, volatile uint8_t *input_ddr, uint8_t input_pin){
   port = input_port;
   ddr = input_ddr;
   pin = input_pin;
}

void tone(uint16_t frequency){
   *ddr |= (1 << pin);
   setFrequency(frequency);
   tone_enabled = true;
}

void mute(){
   *port &= ~(1 << pin);
   tone_enabled = false;
}

ISR(TIMER1_COMPA_vect) {
   if (tone_enabled){
      *port ^= (1 << pin);
   }
}