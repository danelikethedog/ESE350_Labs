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

volatile int ping_state = 0;
volatile int cap_state = 0;
volatile int long pulse_1 = 0;
volatile int long pulse_2 = 0;
volatile int long pulse_width = 0;
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
	switch(cap_state) {
		case 0:
			pulse_1 = ICR1;

			//Edge select low
			TCCR1B &= ~(0x40);

			cap_state = 1;
			//printf("A\n");
			break;

		case 1:
			pulse_2 = ICR1;
			pulse_width = pulse_2 - pulse_1;

			OCR1A = 3200;
			
			
			
			DDRB |= 0x02;
			//PORTB |= 0x02;
			TCCR1B |= 0x40;
			

			//Disable Input Interrupts
			TIMSK1 &= ~(0x20);
			//Enable comp interrupts
			TCNT1 = 0x00;
			TIMSK1 |= 0x02;

			cap_state = 0;
			break;

	}

}

ISR(TIMER1_COMPA_vect) {
	switch(ping_state) {
		case 0:
			//Edge select high
			TCCR1B |= 0x40;
			//Set PB1 as input
			DDRB &= ~(0x02);
			//Pause compare interrupts
			TIMSK1 &= ~(0x02);
			//Enable input interrupts
			TIMSK1 |= 0x20;
			ping_state = 1;

			break;
		case 1:
			PORTB |= 0x02;
			OCR1A = 80;
			ping_state = 0;

			break;
	}

}


int main(void) {


	//Proximity Sensor Set Up
	//PB1 Set as output
	DDRB |= 0x02;
	//Pull PB1 high
	PORTB |= 0x02;
	//Toggle on output compare
	TCCR1A |= 0x80;
	//Unscaled Timer
	TCCR1B |= 0x09;
	//Enable Compare Interrupt
	TIMSK1 |= 0x02;



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
	

	//80 ticks on unscaled is 5us
	OCR1A = 80;

	uart_init();
	sei();


	while(1) {
		//printf("%lu\n", pulse_width);
		//printf("Light: %i", ADC);

		//Set Frequencies
		freqSample = ADC;
		OCR0A = freqCalc(freqSample);
	}

}