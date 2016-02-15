#include <avr/io.h> 
#include <avr/interrupt.h>
#include "util/delay.h" 
#include <stdio.h>
#include <stdlib.h>
#include "uart.h"

volatile int ping_state = 0;
volatile int cap_state = 0;
volatile int long pulse_1 = 0;
volatile int long pulse_2 = 0;
volatile int long pulse_width = 0;


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

			OCR1A += 80;
			
			
			
			DDRB |= 0x02;
			PORTB |= 0x02;
			TCCR1B |= 0x40;
			cap_state = 0;

			//Disable Input Interrupts
			TIMSK1 &= ~(0x20);
			//Enable comp interrupts
			TIMSK1 |= 0x02;
			//printf("B\n");
			break;

	}

}

ISR(TIMER1_COMPA_vect) {

	//Edge select high
	TCCR1B |= 0x40;
	//Set PB1 as input
	DDRB &= ~(0x02);
	//Pause compare interrupts
	TIMSK1 &= ~(0x02);
	//Enable input interrupts
	TIMSK1 |= 0x20;
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
	TCCR1B |= 0x01;
	//Enable Compare Interrupt
	TIMSK1 |= 0x02;



	//Light Sensor Set Up
	DDRC = 0x00;
	ADMUX |= 0x40;
	ADCSRA |= 0xE0;
	ADCSRB = 0x00;

	

	//80 ticks on unscaled is 5us
	OCR1A = 80;

	uart_init();
	sei();


	while(1) {
		//printf("%lu\n", pulse_width);
		printf("Light: %i", ADC);
	}

}