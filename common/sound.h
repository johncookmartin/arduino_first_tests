#include <avr/io.h>

void initSound(volatile uint8_t *input_port, volatile uint8_t *input_ddr, uint8_t input_pin);
void tone(uint16_t frequency);
void mute();