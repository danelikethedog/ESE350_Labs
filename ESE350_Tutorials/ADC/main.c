#include <avr/io.h> 
#include <avr/interrupt.h>
#include "util/delay.h" 
#include <stdio.h>
#include "uart.h"

int main(void) {



	sei();
	uart_init();
	adc_init();


	while(1) {

	}

}