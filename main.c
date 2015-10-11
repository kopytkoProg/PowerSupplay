#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include "Include/Config.h"
#include "uart/uart.h"
#include "adc/adc.h"
#include "pwm/pwm.h"

char cahr_buffer[50];
uint8_t counter = 0;

void set_5v(void) {
	CONTROLL_PORT |= _BV(CONTROLL_5v_PORT);
}

void reset_5v(void) {
	CONTROLL_PORT &= ~_BV(CONTROLL_5v_PORT);
}

//uint8_t is_set_5v(void) {
//	return bit_is_set(CONTROLL_PORT, CONTROLL_5v_PORT); //CONTROLL_PORT & _BV(CONTROLL_5v_PORT);
//}

int main(void) {

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

	// Enable global interrupts
	sei();
	init_uart();
	init_adc();					// have to be after sei() because in other case can't enable interrupt
	init_pwm();

	OCR1_5v = 0;

	DDRB |= 1 << 0;
	while (1) {
		if (OCR1_5v > 0) {
			 OCR1_5v--;
		}

		if (is_ready()) start_all_chanel_converrsion();
		_delay_ms(10);
		sprintf(cahr_buffer, "A0: %u, A1: %u, OCR1_5v: %u", adc_value[0], adc_value[1], OCR1_5v);
		send(cahr_buffer);


	}

}

void onChar(char c) {

}

ISR(INT_5V_vect) {

	// If already disabled the do nothing
	//if (!is_set_5v()) return;

	counter++;

	OCR1_5v = 100;
	reset_5v();

	// Wait after interrupt
	_delay_us(10);
	// Clear pending flag
	GIFR |= _BV(INT_FLAG_5V);

}

ISR(INT_12V_vect) {

}
