#include <avr/io.h> 
#include <avr/interrupt.h>
#include "util/delay.h" 
#include <stdio.h>
#include <stdlib.h>
#include "uart.h"


volatile int comp_state = 0;
volatile int ping_state = 0;
volatile long edge = 0;
volatile long edge2 = 0;
volatile long dif = 0;
volatile int falling = 0;

void ping() {

}

ISR(TIMER1_CAPT_vect) {

	PORTB |= 0x04;
	if (falling) {
		edge = ICR1;
		dif = edge - edge2;
		TCCR1B |= 0x4A;
		falling = 0;
		TIMSK1 &= ~(0x20);
		TIMSK1 |= 0x02;
		

	} else {
		edge2 = ICR1;
		TCCR1B &= ~(0x40);
		falling = 1;
	}
}

ISR(TIMER1_COMPA_vect) {
	switch(comp_state) {
		case 0:
			OCR1A = 42000;
			comp_state = 1;
			PORTB &= ~(0x02);
			DDRB &= ~(0x02);
			TIMSK1 &= ~(0x02);
			TIMSK1 |= 0x20;
			break;
		case 1:
			DDRB |= 0x02;
			OCR1A = 10;
			PORTB |= 0x02;
			comp_state = 0;
			PORTB &= ~(0x04);
			break;
	}
	
}

int main(void) {


	//Proximity Sensor Set Up
	//PB1 Set as output
	DDRB |= 0x06;
	//Pull PB1 high
	PORTB |= 0x02;
	PORTB &= ~(0x04);
	//Clear on output compare
	TCCR1A |= 0x00;
	//Scaled by 8 Timer
	TCCR1B |= 0x4A;
	//10 ticks on unscaled is 5us
	OCR1A = 10;
	//Enable Compare Interrupt
	TIMSK1 |= 0x02;
	

	uart_init();
	sei();


	while(1) {
		// ping();
		printf("Edge Dif: %lu\n", dif);
		_delay_ms(500);
		

	}

}