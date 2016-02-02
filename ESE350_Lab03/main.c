#include <avr/io.h> 
#include <avr/interrupt.h>
#include "util/delay.h" 
#include <stdio.h>
#include "uart.h"


//OC1A used as PB1
int HiOrLo;
long unsigned stallTime;
 

ISR(TIMER1_COMPA_vect) {

	OCR1A += stallTime;

}

long freqCalc(int inHertz) {

	int unsigned top = inHertz;
	return 16000000/top;
}
char* getArgument() {
	char* retVal[8];
	scanf("%s", &retVal);
	
	return retVal;
	
	
}

int main(void) {

	//stallTime = 18073;

	DDRB |= 0xFF;

	uart_init();

	//PORTB |= 0x02;

	TCCR1A = 0x40;

	TCCR1B = 0x01;

	TIMSK1 = 0x02;

	OCR1A = TCNT1 + 16;

	stallTime = freqCalc(1200)/2;



	

	sei();
	//printf(getArgument());
	while(1) {
		//printf("%lu\n", stallTime);
	}

}