/*
 * timer.c
 *
 *  Created on: 30 lis 2015
 *      Author: michal
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

uint64_t TIMEus = 0;

void init_timer(void) {


	TCCR0 |= _BV(CS02) | _BV(CS00); 		// źródłem CLK, preskaler 1024
	TCCR0 |= _BV(WGM01);								// CTC

	OCR0 = 160;

	TIMSK |= (1 << OCIE0);           			//Przerwanie compare

	// W tej konfiguracji 16 000 000 / 1024 / 160 = 97,65625 mamy co 1/97,65625 = 0,01024s

}

ISR(TIMER0_COMP_vect) {
	TIMEus += 10240;
}
