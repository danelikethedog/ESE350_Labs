#include <avr/io.h> 
#include <avr/interrupt.h>
#include "util/delay.h" 
#include <stdio.h>
#include "uart.h"


volatile int sample = 0;

int main(void) {



	//Light Sensor Set Up
	DDRC = 0x00;
	ADMUX |= 0x40;
	ADCSRA |= 0xE0;
	ADCSRB = 0x00;

	DDRB = 0x20;

	sei();
	uart_init();


	while(1) {
		sample = ADC;
		if (sample < 512) {
			PORTB = 0x00;
		} else {
			PORTB = 0x20;
		}

		printf("Value: %i\n", sample);
	}

}