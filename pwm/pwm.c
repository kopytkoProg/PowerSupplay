#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "pwm.h"

void init_pwm(void) {

	// OC1A set on compare
	TCCR1A |= _BV(COM1A0) | _BV(COM1A1);

	// OC1B set on compare
	TCCR1A |= _BV(COM1B0) | _BV(COM1B1);

	// Fast pwm mode 14
	TCCR1A |= _BV(WGM11);
	TCCR1B |= _BV(WGM12) | _BV(WGM13);

	// No prescaler
	TCCR1B |= _BV(CS10);

	// Limit
	ICR1 = 1000;

	OCR1A = 100;
	OCR1B = 100;

}


/***
 * IF 0 then 100% high
 * IF 1000 then 0% high
 */
void set_OCR1A(uint16_t v){
	OCR1A = v;
}

/***
 * IF 0 then 100% high
 * IF 1000 then 0% high
 */
void set_OCR1B(uint16_t v){
	OCR1B = v;
}
