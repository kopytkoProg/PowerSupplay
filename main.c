#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include "Include/Config.h"
#include "uart/uart.h"
#include "adc/adc.h"

char cahr_buffer[50];
uint8_t counter = 0;

void set_5v(void) {
	CONTROLL_PORT |= _BV(CONTROLL_5v_PORT);
}

void reset_5v(void) {
	CONTROLL_PORT &= ~_BV(CONTROLL_5v_PORT);
}

uint8_t is_set_5v(void) {
	return bit_is_set(CONTROLL_PORT, CONTROLL_5v_PORT); //CONTROLL_PORT & _BV(CONTROLL_5v_PORT);
}

int main(void) {

	init_uart();

	init_adc();

	CONTROLL_DDR |= _BV(CONTROLL_12v_DDD) | _BV(CONTROLL_5v_DDD);
	// ======== INT0, INT1 ========
	// Both on rising edge
	MCUCR |= _BV(ISC10) | _BV(ISC11) | _BV(ISC00) | _BV(ISC01);

	// Enable INT0, INT1
	GICR |= _BV(INT0) | _BV(INT1);
	// ============================

	// ADC
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

	// Enable 5V
	set_5v();

	sei();
	// Enable global interrupts

	DDRB |= 1 << 0;
	while (1) {
		if (!is_set_5v()) {

			sprintf(cahr_buffer, "Waiting for power on (%u)", counter);
			send(cahr_buffer);

			_delay_ms(1000);
			send("Power on");
			set_5v();

		}

		sprintf(cahr_buffer, "ADC0: %u, ADC1: %u", adc_read(0), adc_read(1));
		send(cahr_buffer);
	}

}

void onChar(char c) {

}

ISR(INT_5V_vect) {

	// If already disabled the do nothing
	if(!is_set_5v()) return;

	counter++;
	reset_5v();

	// Wait after interrupt
	_delay_us(10);
	// Clear pending flag
	GIFR |= _BV(INT_FLAG_5V);


}

ISR(INT_12V_vect) {

}
