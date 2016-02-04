#include <avr/io.h> 
#include <avr/interrupt.h>
#include "util/delay.h" 
#include <stdio.h>
#include <stdlib.h>
#include "uart.h"

//Define Frequencies
#define Freq1C 1209
#define Freq2C 1336
#define Freq3C 1477
#define FreqAC 1633
#define Freq1R 697
#define Freq4R 770
#define Freq7R 852
#define FreqStarR 941



//OC1A used as PB1
int HiOrLo;
long unsigned rowStallTime;
long unsigned columnStallTime;
char next_char;
int iterate;
char first[8];
char second[8];
int rowHolder;
char keyPress;

/*
Need to create a while loop for while key is depressed in each if statement
or branch to helper function and play out frequency on OC1A and OC1B.
*/


ISR(TIMER2_COMPA_vect) {
	if (rowHolder == 1) {

		if (PIND & 0x04) {
			keyPress = '1';
			//printf("1");
		} else if (PIND & 0x08) {
			keyPress = '2';
			//printf("2");
		} else if (PIND & 0x10) {
			keyPress = '3';
			//printf("3");
		} else if (PIND & 0x20) {
			keyPress = 'A';
			//printf("A");
		}

		rowHolder++;

		PORTC &= ~(1 << PC2);
		PORTC |= (1 << PC3);
		PORTC &= ~(1 << PC4);
		PORTC &= ~(1 << PC5);

	} else if (rowHolder == 2) {
		

		if (PIND & 0x04) {
			keyPress = '4';
			//printf("4");
		} else if (PIND & 0x08) {
			keyPress = '5';
			//printf("5");
		} else if (PIND & 0x10) {
			keyPress = '6';
			//printf("6");
		} else if (PIND & 0x20) {
			keyPress = 'B';
			//printf("B");
		}

		rowHolder++;

		PORTC |= (1 << PC4);
		PORTC &= ~(1 << PC2);
		PORTC &= ~(1 << PC3);
		PORTC &= ~(1 << PC5);

	} else if (rowHolder == 3) {

		

		if (PIND & 0x04) {
			keyPress = '7';
			//printf("7");
		} else if (PIND & 0x08) {
			keyPress = '8';
			//printf("8");
		} else if (PIND & 0x10) {
			keyPress = '9';
			//printf("9");
		} else if (PIND & 0x20) {
			keyPress = 'C';
			//printf("C");
		}

		rowHolder++;

		PORTC &= ~(1 << PC2);
		PORTC &= ~(1 << PC3);
		PORTC &= ~(1 << PC4);
		PORTC |= (1 << PC5);


	} else if (rowHolder == 4) {

		if (PIND & 0x04) {
			keyPress = '*';
			//printf("*");
		} else if (PIND & 0x08) {
			keyPress = '0';
			//printf("0");
		} else if (PIND & 0x10) {
			keyPress = '#';
			//printf("#");
		} else if (PIND & 0x20) {
			keyPress = 'D';
			//printf("D");
		}

		rowHolder = 1;

		PORTC |= (1 << PC2);
		PORTC &= ~(1 << PC3);
		PORTC &= ~(1 << PC4);
		PORTC &= ~(1 << PC5);
	}

}

ISR(TIMER1_COMPA_vect) {

	OCR1A += rowStallTime;

}

ISR(TIMER1_COMPB_vect) {

	OCR1B += columnStallTime;

}

long freqCalc(long inHertz) {
	return 16000000/inHertz;
}


int main(void) {

	//stallTime = 18073;

	DDRB |= 0xFF;
	DDRC |= 0xFF;
	DDRD |= 0x00;
	PORTC = 0x00;
	rowHolder = 1;
	rowStallTime = 0;
	columnStallTime = 0;

	//Set rows early to prevent timing error
	PORTC |= (1 << PC2);
	PORTC &= ~(1 << PC3);
	PORTC &= ~(1 << PC4);
	PORTC &= ~(1 << PC5);

	uart_init();


	//Timer 1 Config
	TCCR1A = 0x40;

	TCCR1B = 0x01;

	TIMSK1 = 0x02;

	OCR1A = TCNT1 + 16;


	//Timer 2 Config

	TCCR2A |= 0x02;
	TCCR2B |= 0x05;
	TIMSK2 |= 0x02;
	OCR2A = 0xFF;


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