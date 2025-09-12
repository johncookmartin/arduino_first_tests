#include <stdint.h>
#include <stdbool.h>

void initADC(bool use_8_bit);
uint16_t analog(uint8_t channel, bool is_left);