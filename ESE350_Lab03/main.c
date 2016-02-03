#include <avr/io.h> 
#include <avr/interrupt.h>
#include "util/delay.h" 
#include <stdio.h>
#include <stdlib.h>
#include "uart.h"


//OC1A used as PB1
int HiOrLo;
long unsigned stallTime;
char next_char;
int iterate;
char first[8];
char second[8];
char *charHolder;
 

ISR(TIMER1_COMPA_vect) {

	OCR1A += stallTime;

}

long freqCalc(long inHertz) {
	return 16000000/inHertz;
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

	//Get the first number, break on space, and get the second number
	scanf("%s", &first);
	scanf("%s", &second);


	printf("First: %s\n", first);
	printf("Second: %s\n", second);

	//Convert the strings collected to longs
	long unsigned freqInput = strtol(&first, NULL, 10);
	long unsigned durationInput = strtol(&second, NULL, 10);

	//Test print statements for the input freq and duration
	printf("Freq: %i\n", freqInput);
	printf("Dur: %i", durationInput);

	//Calculate the stall time
	stallTime = freqCalc(freqInput)/2;

	printf("StallTime: %lu\n", stallTime);


	sei();



	while(1) {

	}

}