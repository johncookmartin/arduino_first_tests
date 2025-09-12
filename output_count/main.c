
// AVR C version for Atmega328p (no Arduino libraries)
#define F_CPU 16000000UL
#include "../common/serial.h"
#include "../common/counter.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>

/* Connection Diagram
Atmega328p     Romeo board      IO Board Jumper     Component
PB2 ->         D10 ->           JP2_5 ->            S1
PB3 ->         D11 ->           JP2_6 ->            S2
PB4 ->         D12 ->           JP2_7 ->            S3
*/

int bit_counter(int prev, int max, int min){
	if (!(PIND & (1 << PD3))) {
		_delay_ms(50);
		if (!(PIND & (1 << PD3))) {
			prev = increment(prev, max, min);
			while(!(PIND & (1 << PD3)));
		}
	}

	if (!(PIND & (1 << PD4))) {
		_delay_ms(50);
		if (!(PIND & (1 << PD4))) {
			prev = decrement(prev, max, min);
			while(!(PIND & (1 << PD4)));
		}
	}

	return prev;
}

bool count_mode_reader(bool prev){
	bool result = prev;
	if (!(PIND & (1 << PD2))) {
		_delay_ms(50);
		if (!(PIND & (1 << PD2))) {
			result = !prev;
			PORTB ^= (1 << PB5);
			while(!(PIND & (1 << PD2)));
		}
	}
	return result;
}

uint8_t bit_display(){
	uint8_t current = 0;
	if (!(PIND & (1 << PD5))){
		_delay_ms(50);
		current |= 1 << 3;
	}
	if (!(PIND & (1 << PD6))){
		_delay_ms(50);
		current |= 1 << 2;
	}
	if (!(PIND & (1 << PD7))){
		_delay_ms(50);
		current |= 1 << 1;
	}
	if (!(PINB & (1 << PB0))){
		_delay_ms(50);
		current |= 1;
	}
	return current;
}

int main(void) {
	// Initialize
	initUART(9600);
	PORTD = 0b11111100; // pull-ups
	PORTB = 0b00000001;
	DDRB |= (1 << PB5); // LED output

	transmitString("Welcome to Serial Counter!");
	transmitString("Press S3 to switch between momentary and dip counting");
	transmitString("Currently set to momentary counting");

	int current = 0;
	bool momentary_counting = true;

	while (1) {
		bool new_setting = count_mode_reader(momentary_counting);
		if (new_setting != momentary_counting){
			if (new_setting){
				transmitString("Changing to momentary counting");
			}
			else {
				transmitString("Changing to DIP Counting");
			}
			momentary_counting = new_setting;
		}

		if (momentary_counting){
			int prev_current = current;
			current = bit_counter(current, 9999, -9999);
			if (prev_current != current){
				printDec(current);
			}
		}
		else {
			int prev_current = current;
			current = 0;
			char binary[] = "00000000";
			if (!(PIND & (1 << PD5))){
				_delay_ms(50);
				current |= 1 << 3;
				binary[4] = '1';
			}
			if (!(PIND & (1 << PD6))){
				_delay_ms(50);
				current |= 1 << 2;
				binary[5] = '1';
			}
			if (!(PIND & (1 << PD7))){
				_delay_ms(50);
				current |= 1 << 1;
				binary[6] = '1';
			}
			if (!(PINB & (1 << PB0))){
				_delay_ms(50);
				current |= 1;
				binary[7] = '1';
			}
			if (prev_current != current){
				char buf[16];
				snprintf(buf, sizeof(buf), "%s %d", binary, current);
				transmitString(buf);
			}
		}
	}
	return 0;
}
