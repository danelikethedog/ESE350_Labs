#include <avr/io.h> 
#include <avr/interrupt.h>
#include "util/delay.h" 
#include <stdio.h>
#include "uart.h"


//OC1A used as PB1
int HiOrLo, stallTime;
 

ISR(TIMER1_COMPA_vect) {

	OCR1A += stallTime;

}

int freqCalc(int inHertz) {
	return (1/inHertz)*16*100000;
}

int main(void) {

	stallTime = 18073;

	DDRB |= 0xFF;

	//PORTB |= 0x02;

	TCCR1A = 0x40;

	TCCR1B = 0x01;

	TIMSK1 = 0x02;

	OCR1A = TCNT1 + 16;

	sei();

	while(1);

}