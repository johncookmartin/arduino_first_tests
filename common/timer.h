#include <stdint.h>

enum timer_prescaler {
   PRESCALE_0 = 0,
   PRESCALE_1 = 1,
   PRESCALE_8 = 8,
   PRESCALE_64 = 64,
   PRESCALE_256 = 256,
   PRESCALE_1024 = 1024
};

void initTimer(uint16_t frequency, enum timer_prescaler divider);
void setFrequency(int new_frequency);