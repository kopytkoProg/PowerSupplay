#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "adc.h"

typedef enum {
	Idle, InProgres
} _t_adc_status;

volatile static uint8_t channel = 0;
volatile static _t_adc_status status = Idle;

void init_adc(void) {
	uint8_t i;

	for (i = 0; i < ADC_CHANELS; ++i) {
		adc_value[i] = 0;
	}

	// AREF = AVcc
	ADMUX = (1 << REFS0);

	// ADC Enable and prescaler of 128
	// 16000000/128 = 125000
	ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);

	ADCSRA |= _BV(ADIE);

}

void start_all_chanel_converrsion(void) {

	if (status != Idle) return;

	status = InProgres;

	// reset channel
	channel = 0;
	ADMUX = (ADMUX & 0xF8) | channel;

	// start single conversion
	ADCSRA |= (1 << ADSC);
}

uint8_t is_ready(void) {
	return status == Idle;
}

ISR(ADC_vect) {

	// save data
	adc_value[channel] = ADC;

	// select next channel
	channel++;
	// if all channel done then end
	if (channel >= ADC_CHANELS) {
		status = Idle;
		return;
	}

		// select channel
		ADMUX = (ADMUX & 0xF8) | channel;

		// start single conversion
		ADCSRA |= (1 << ADSC);


}

//uint16_t adc_read(uint8_t ch) {
//	// select the corresponding channel 0~7
//	// ANDing with ’7′ will always keep the value
//	// of ‘ch’ between 0 and 7
//	ch &= 0b00000111;  // AND operation with 7
//	ADMUX = (ADMUX & 0xF8) | ch; // clears the bottom 3 bits before ORing
//
//	// start single convertion
//	// write ’1′ to ADSC
//	ADCSRA |= (1 << ADSC);
//
//	// wait for conversion to complete
//	// ADSC becomes ’0′ again
//	// till then, run loop continuously
//	while (ADCSRA & (1 << ADSC))
//		;
//
//	return (ADC);
//}
