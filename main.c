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
#include "lcd/hd44780.h"
#include "jtag/jtag.h"

char cahr_buffer[200];
uint8_t counter = 0;

int main(void) {

	JTAG_is_IO(); // Disable JTAG

	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

	// Enable global interrupts
	sei();
	init_uart();
	init_adc();					// have to be after sei() because in other case can't enable interrupt
	init_pwm();

	_delay_ms(1000);
	lcd_init();
	lcd_clrscr();

	DDRD |= _BV(DDB4) | _BV(DDB5);
	set_OCR1A(1000 - 200);

	while (1) {

		if (is_ready()) start_all_chanel_converrsion();
		_delay_ms(10);

		sprintf(cahr_buffer, "C1: %f, C2: %f, C3: %f, C3: %f, C5: %f, C6: %f",
				adc_value[VOLTAGE_V5] / 1023.0 * VREF,
				adc_value[VOLTAGE_V12] / 1023.0 * VREF,
				adc_value[CURRENT_V5] / 1023.0 * VREF,
				adc_value[CURRENT_V12] / 1023.0 * VREF,
				adc_value[MAX_CURRENT_V12] / 1023.0 * VREF,
				adc_value[MAX_CURRENT_V5] / 1023.0 * VREF
		);
		send(cahr_buffer);

		float volt_line_12 = adc_value[VOLTAGE_V12] / 1023.0 * VREF / SCALE_12V;
		float volt_line_5 = adc_value[VOLTAGE_V5] / 1023.0 * VREF / SCALE_5V;

		float amp_actual_line_12 = adc_value[CURRENT_V12] / 1023.0 * VREF / SCALE_OPAMP / RESISTOR;
		float amp_actual_line_5 = adc_value[CURRENT_V5] / 1023.0 * VREF / SCALE_OPAMP / RESISTOR;

		float amp_max_line_12 = adc_value[MAX_CURRENT_V12] / 1023.0 * VREF / SCALE_OPAMP / RESISTOR;
		float amp_max_line_5 = adc_value[MAX_CURRENT_V5] / 1023.0 * VREF / SCALE_OPAMP / RESISTOR;

		lcd_goto(0);
		lcd_putsf(LCD_FORMAT, volt_line_5, amp_actual_line_5, amp_max_line_5);
		lcd_goto(40);
		lcd_putsf(LCD_FORMAT, volt_line_12, amp_actual_line_12, amp_max_line_12);
	}

}

void onChar(char c) {

}

