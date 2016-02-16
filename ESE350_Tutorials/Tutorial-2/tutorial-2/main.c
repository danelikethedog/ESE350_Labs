/*
 * tutorial-2.c
 *
 * Created: 2/16/2016 1:59:40 PM
 * Author : William
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
//#include "uart.h"


int adc_value;


ISR(ADC_vect) {
	printf("%u\n", ADC);
}

int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
		/*
		//set reference voltage to 5V
		ADMUX |= 0x40;
		//Set Analog Channel
		ADMUX |= 0x05;
		//Start conversion
		ADCSRA |= 0x40;
		//Choosing trigger source
		ADCSRA |= 0x20;
		//Turn on Free Running Mode
		ADCSRB |= 0x00;
		//No prescalar clock speed
		ADCSRA |= 0x00;
		*/
		
		ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));
		ADMUX |= (1<<REFS0);
		ADMUX &= ~(1<<REFS1);
		ADCSRB &= ~((1<<ADTS2)|(1<<ADTS1)|(1<<ADTS0));
		ADCSRA |= (1<<ADATE);
		ADCSRA |= (1<<ADEN);
		ADCSRA |= (1<<ADSC);
		
		sei();
		while(1) {}
			//hi
    }
	
	return 0;
}

