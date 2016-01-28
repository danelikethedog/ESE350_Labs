#include <avr/io.h> 
#include <avr/interrupt.h>
#include "util/delay.h" 
#include <stdio.h>
#include "uart.h"


//OC1A used as PB1
int HiOrLo;
long stallTime;
 

ISR(TIMER1_COMPA_vect) {

	OCR1A += stallTime;

}

float freqCalc(int inHertz) {
	float top, bottom;
	top = 1/inHertz;
	bottom = 1/16000000;
	return top/bottom;
}
char* getArgument() {
	char* retVal[8];
	scanf("%s", &retVal);
	
	return retVal;
	
	
}

int main(void) {

	//stallTime = 18073;

	DDRB |= 0xFF;

	//PORTB |= 0x02;

	TCCR1A = 0x40;

	TCCR1B = 0x01;

	TIMSK1 = 0x02;

	OCR1A = TCNT1 + 16;

	stallTime = freqCalc(1000)/2;

	

	sei();
	printf(getArgument());
	while(1);

}