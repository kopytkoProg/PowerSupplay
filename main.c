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

char cahr_buffer[50];
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

		sprintf(cahr_buffer, "V: %u, A: %u", adc_value[VOLTAGE_V5] , adc_value[CURRENT_V5] );
		send(cahr_buffer);


		float volt_line_12 = adc_value[VOLTAGE_V12] / 1023.0 * VREF; // SCALE_12V;
		float volt_line_5 = adc_value[VOLTAGE_V5] / 1023.0 * VREF; // SCALE_5V;



		lcd_goto(0);
		lcd_putsf(LCD_FORMAT, volt_line_5, 1.0, 2.0);
		lcd_goto(40);
		lcd_putsf(LCD_FORMAT, volt_line_12, 1.0, 2.0);
	}

}

void onChar(char c) {

}

