/*
 * buzer.c
 *
 *  Created on: 2 gru 2015
 *      Author: michal
 */


#include <avr/io.h>
#include <stdio.h>



void init_buzer(void) {
	DDRD |= _BV(DDD2)| _BV(DDD3);
}


void buzer_on(void) {
	PORTD |= _BV(PORTD2);
}

void buzer_togle(void) {
	PORTD ^= _BV(PORTD2);
}


void buzer_off(void) {
	PORTD &= ~_BV(PORTD2);
}
