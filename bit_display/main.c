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
PB2 ->         D10 ->           JP2_5 ->            DS_1                             
PB3 ->         D11 ->           JP2_6 ->            DS_2                             
PB4 ->         D12 ->           JP2_7 ->            DS_3                           
PB5 ->         D13 ->                               L1

*/

int main(void) {
    // Setup
    DDRD = 0b11111100; // PD2-PD7 as output
    DDRB = 0b00000011; // PB0, PB1 as output
    PORTB = 0b00011100; // Internal pull-ups on PB2, PB3, PB4

    while (1) {
        uint8_t current = 0;

        if (!(PINB & (1 << PB2))) {
            current |= 1 << 2;
        }
        if (!(PINB & (1 << PB3))) {
            current |= 1 << 1;
        }
        if (!(PINB & (1 << PB4))) {
            current |= 1;
        }

        uint8_t display = 0;
        switch (current) {
            case 1: display = 0b00000001; break;
            case 2: display = 0b00000011; break;
            case 3: display = 0b00000111; break;
            case 4: display = 0b00001111; break;
            case 5: display = 0b00011111; break;
            case 6: display = 0b00111111; break;
            case 7: display = 0b01111111; break;
            default: display = 0; break;
        }

        uint8_t pd_bits = (display << 2);
        PORTD = (PORTD & 0b00000011) | pd_bits;

        uint8_t pb_low = (display >> 6) & 0b00000011;
        PORTB = (PORTB & 0b11100000) | 0b00011100 | pb_low;

        _delay_ms(50);
    }
}