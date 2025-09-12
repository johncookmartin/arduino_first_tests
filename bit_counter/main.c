#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

/* Connection Diagram

Atmega328p     Romeo board      IO Board Jumper     Component
PD2 ->         D2  ->           JP3_1 ->            D1A                             
PD3 ->         D3  ->           JP3_2 ->            D1B                             
PD4 ->         D4  ->           JP3_3 ->            D1C                             
PD5 ->         D5  ->           JP3_4 ->            D1D                             
PD6 ->         D6  ->           JP3_5 ->            D1E                             
PD7 ->         D7  ->           JP3_6 ->            D1F                             
PB0 ->         D8  ->           JP3_7 ->            D1G                             
PB1 ->         D9  ->           JP3_8 ->            D1H                             
PB2 ->         D10 ->           JP2_5 ->            S1                             
PB3 ->         D11 ->           JP2_6 ->            S2                             
PB4 ->         D12 ->           JP2_7 ->            S3                           
PB5 ->         D13 ->                               L1

*/

uint8_t increment(uint8_t value, uint8_t max) {
    if (value == max) {
        return 0;
    } else {
        return value + 1;
    }
}

uint8_t decrement(uint8_t value, uint8_t max) {
    if (value == 0){
        return max;
    } else {
        return value - 1;
    }
}

void auto_bit_counter(uint8_t max){
    uint8_t prev = max;
    do {
        uint8_t current = increment(prev, max);
        PORTD = current << 2;
        PORTB = current >> 6;
        _delay_ms(1000);
        prev = current;
    } while (prev < max);
}

uint8_t bit_counter(uint8_t prev, uint8_t max){
    if (!(PINB & (1 << PB2))) {
        _delay_ms(50);
        if (!(PINB & (1 << PB2))) {
            prev = increment(prev, max);
            PORTB ^= (1 << PB5);
            while(!(PINB & (1 << PB2)));
        }
    }
    if (!(PINB & (1 << PB3))) {
        _delay_ms(50);
        if (!(PINB & (1 << PB3))) {
            prev = decrement(prev, max);
            PORTB ^= (1 << PB5);
            while(!(PINB & (1 << PB3)));
        }
    }
    if (!(PINB & (1 << PB4))) {
        _delay_ms(50);
        if (!(PINB & (1 << PB4))) {
            prev = decrement(prev, max);
            PORTB ^= (1 << PB5);
            while(!(PINB & (1 << PB4)));
        }
    }
    return prev;
}

int main(void) {
    // Setup (was setup())
    DDRD = 0b11111100;
    DDRB = 0b00100011; // Set PB5 as output for LED, PB0, PB1 as output
    PORTB = 0b00011100; // Internal pull-ups
    DDRB |= (1 << PB5); // Set LED for debug

    uint8_t current = 0;

    while (1) {
        // auto_bit_counter(255);
        current = bit_counter(current, 255);

        uint8_t currentB = current >> 6;
        currentB |= 7 << 2;

        PORTD = current << 2;
        PORTB = currentB;
    }
}