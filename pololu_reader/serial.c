#include "serial.h"
#include <avr/io.h>
#include <stdio.h>

void initUART(unsigned int baud) {
	unsigned int ubrr = F_CPU/8/baud-1;
	UCSR0A |= (1 << U2X0);
	UBRR0H = (unsigned char) (ubrr >> 8);
	UBRR0L = (unsigned char) ubrr;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (1 <<UCSZ00) | (1 <<UCSZ01);
}

void transmitByte (unsigned char data) {
	while ( !(UCSR0A & (1 << UDRE0)) );
	UDR0 = data;
}

unsigned char receiveByte ( void ) {
	while ( !(UCSR0A &  (1 << RXC0)) );
	return UDR0;
}

void transmitString (const char* str) {
	while (*str) {
		transmitByte(*str++);
	}
	transmitByte('\r');
	transmitByte('\n');
}

void printDec(int num){
	char buf[8];
	snprintf(buf, sizeof(buf), "%d", num);
	transmitString(buf);
}