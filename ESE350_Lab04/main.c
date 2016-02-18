#include <avr/io.h> 
#include <avr/interrupt.h>
#include "util/delay.h" 
#include <stdio.h>
#include <stdlib.h>
#include "uart.h"


#define freq1 1000
#define freq2 1122
#define freq3 1260
#define freq4 1335
#define freq5 1498
#define freq6 1682
#define freq7 1888
#define freq8 2000


volatile int comp_state = 0;
volatile int ping_state = 0;
volatile long edge = 0;
volatile long edge2 = 0;
volatile long dif = 0;
volatile int falling = 0;

volatile int freqSample = 0;


short int freqCalc(long inHertz) {
	short int returnVal;

	returnVal = inHertz < 431 ? freq1 :
				inHertz < 479 ? freq2 :
				inHertz < 527 ? freq3 :
				inHertz < 575 ? freq4 :
				inHertz < 623 ? freq5 :
				inHertz < 671 ? freq6 :
				inHertz < 719 ? freq7 :
				freq8;

	return 250000/(2*returnVal);
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

	//Light Sensor Set Up
	DDRC = 0x00;
	ADMUX |= 0x40;
	ADCSRA |= 0xE0;
	ADCSRB = 0x00;


	//Frequency Set Up
	TCCR0A = 0x42;
	TCCR0B = 0x03;
	TIMSK0 = 0x00;
	DDRD |= 0x40;

	//Proximity Sensor Set Up
	//PB1 Set as output
	DDRB |= 0x1E;
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
		

		freqSample = ADC;
		OCR0A = freqCalc(freqSample);

		if (dif < 2075) {
			PORTB &= ~(0x1C);
		} else if (dif < 2350) {
			PORTB &= ~(0x18);
			PORTB |= 0x04;
		} else if (dif < 2625) {
			PORTB &= ~(0x14);
			PORTB |= 0x08;
		} else if (dif < 2900) {
			PORTB &= ~(0x10);
			PORTB |= (0x0C);
		} else if (dif < 3175) {
			PORTB &= ~(0x0C);
			PORTB |= 0x10;
		} else if (dif < 3450) {
			PORTB &= ~(0x08);
			PORTB |= 0x14;
		} else if (dif < 3725) {
			PORTB &= ~(0x04);
			PORTB |= 0x18;
		} else {
			PORTB |= 0x1C;
		}
 
	}

}