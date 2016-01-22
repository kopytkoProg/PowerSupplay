#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <math.h>
#include "Include/Config.h"
#include "uart/uart.h"
#include "adc/adc.h"
#include "pwm/pwm.h"
#include "lcd/hd44780.h"
#include "jtag/jtag.h"
#include "buzer/buzer.h"
#include "timer/timer.h"

void display(void);
void calculate(void);
void init(void);
void protection(void);
void fan(void);

//char cahr_buffer[200];
uint8_t counter = 0;

float volt_line_12;
float volt_line_5;
float amp_actual_line_12;
float amp_actual_line_5;
float amp_max_line_12;
float amp_max_line_5;

void init(void) {
	JTAG_is_IO(); // Disable JTAG

	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

	// Enable global interrupts
	sei();
	init_uart();
	init_adc();					// have to be after sei() because in other case can't enable interrupt
	init_pwm();
	init_buzer();

	buzer_on();
	_delay_ms(100);
	buzer_off();

	init_timer();

	lcd_init();
	lcd_clrscr();

}

int main(void) {

	init();

	while (1) {

		calculate();
		display();
		protection();
		fan();

	}

}

//==================================================================================================
//==================================================================================================
//==================================================================================================
#define FAN_SPEED_FACTOR			(1.0)
#define FAN_MAX_AVG_POWER_12 		(70.0 * FAN_SPEED_FACTOR)
#define FAN_MAX_AVG_POWER_5 		(35.0 * FAN_SPEED_FACTOR)

void fan(void) {
	static uint64_t last_time = 0;
	static float duty = 0.5;

	static float p_avg_12v = 0;
	static float p_avg_5v = 0;
	static uint16_t measure_num = 0;

	//======================================================================

	measure_num++;
	p_avg_12v += (12.0f - volt_line_12) * amp_actual_line_12;
	p_avg_5v += (5.0f - volt_line_5) * amp_actual_line_5;

	//======================================================================
	if (TIMEus - last_time < 1000L * 100L) return;

	float actualDuty = fmaxf((p_avg_12v / measure_num) / FAN_MAX_AVG_POWER_12, (p_avg_5v / measure_num) / FAN_MAX_AVG_POWER_5);

	// Update duty
	if (duty > 0.5f) duty = fmaxf(duty - 0.005, 0);
	if (actualDuty > duty) duty += (actualDuty - duty) * 0.05;

	duty_OCR1A(duty);

	measure_num = 0;
	p_avg_12v = 0;
	p_avg_5v = 0;

	last_time = TIMEus;
}

//==================================================================================================
//==================================================================================================
//==================================================================================================

#define CYCLIC_TABLE_SIZE 				(50)
#define PROTECTION_MAX_AVG_POWER_12 	(70.0)
#define PROTECTION_MAX_AVG_POWER_5 		(35.0)

void protection(void) {
	static uint64_t last_time = 0;

	static float cyclic_table_12v[CYCLIC_TABLE_SIZE];
	static float cyclic_table_5v[CYCLIC_TABLE_SIZE];

	static uint16_t i_12v = 0;
	static uint16_t i_5v = 0;

	static uint16_t measure_num = 0;

	static float p_avg_12v = 0;
	static float p_avg_5v = 0;

	//======================================================================

	measure_num++;
	p_avg_12v += (12.0f - volt_line_12) * amp_actual_line_12;
	p_avg_5v += (5.0f - volt_line_5) * amp_actual_line_5;

	//======================================================================
	if (TIMEus - last_time < 1000L * 200L) return;				// Each 0.2 s go inside

	cyclic_table_12v[i_12v = (i_12v + 1) % CYCLIC_TABLE_SIZE] = p_avg_12v / measure_num;
	cyclic_table_5v[i_5v = (i_5v + 1) % CYCLIC_TABLE_SIZE] = p_avg_5v / measure_num;

	measure_num = 0;
	p_avg_12v = 0;
	p_avg_5v = 0;

	uint16_t i = 0;
	uint16_t over_power_12 = 0;
	uint16_t over_power_5 = 0;

	for (i = 0; i < CYCLIC_TABLE_SIZE; ++i) {
		if (cyclic_table_12v[i] > PROTECTION_MAX_AVG_POWER_12) over_power_12++;
		if (cyclic_table_5v[i] > PROTECTION_MAX_AVG_POWER_5) over_power_5++;
	}

	if (over_power_12 > CYCLIC_TABLE_SIZE / 2 || over_power_5 > CYCLIC_TABLE_SIZE / 2) {
		buzer_togle();
	} else {
		buzer_off();
	}

	last_time = TIMEus;
}

//==================================================================================================
//==================================================================================================
//==================================================================================================

void calculate(void) {
	if (!is_ready()) return;

//	sprintf(cahr_buffer, "C1: %f, C2: %f, C3: %f, C3: %f, C5: %f, C6: %f",
//			adc_value[VOLTAGE_V5] / 1023.0 * VREF,
//			adc_value[VOLTAGE_V12] / 1023.0 * VREF,
//			adc_value[CURRENT_V5] / 1023.0 * VREF,
//			adc_value[CURRENT_V12] / 1023.0 * VREF,
//			adc_value[MAX_CURRENT_V12] / 1023.0 * VREF,
//			adc_value[MAX_CURRENT_V5] / 1023.0 * VREF
//	);
//	send(cahr_buffer);

	volt_line_12 = adc_value[VOLTAGE_V12] / 1023.0 * VREF / SCALE_12V;
	volt_line_5 = adc_value[VOLTAGE_V5] / 1023.0 * VREF / SCALE_5V;

	amp_actual_line_12 = adc_value[CURRENT_V12] / 1023.0 * VREF / SCALE_OPAMP / RESISTOR;
	amp_actual_line_5 = adc_value[CURRENT_V5] / 1023.0 * VREF / SCALE_OPAMP / RESISTOR;

	amp_max_line_12 = adc_value[MAX_CURRENT_V12] / 1023.0 * VREF / SCALE_OPAMP / RESISTOR;
	amp_max_line_5 = adc_value[MAX_CURRENT_V5] / 1023.0 * VREF / SCALE_OPAMP / RESISTOR;

	start_all_chanel_converrsion();
}

//==================================================================================================
//==================================================================================================
//==================================================================================================

void display(void) {
	static uint64_t last_time = 0;

	static uint16_t measure_num = 0;

	static float avg_volt_12v = 0;
	static float avg_volt_5v = 0;
	static float avg_amp_12v = 0;
	static float avg_amp_5v = 0;
	static float avg_amp_max_12v = 0;
	static float avg_amp_max_5v = 0;

	//======================================================================

	measure_num++;

	avg_volt_12v += volt_line_12;
	avg_volt_5v += volt_line_5;
	avg_amp_12v += amp_actual_line_12;
	avg_amp_5v += amp_actual_line_5;
	avg_amp_max_12v += amp_max_line_12;
	avg_amp_max_5v += amp_max_line_5;

	//======================================================================
	if (TIMEus - last_time < 1000L * 100L) return;

	lcd_goto(0);
	lcd_putsf(LCD_FORMAT, avg_volt_5v / measure_num, avg_amp_5v / measure_num, avg_amp_max_5v / measure_num);
	lcd_goto(40);
	lcd_putsf(LCD_FORMAT, avg_volt_12v / measure_num, avg_amp_12v / measure_num, avg_amp_max_12v / measure_num);

	measure_num = 0;

	avg_volt_12v = 0;
	avg_volt_5v = 0;
	avg_amp_12v = 0;
	avg_amp_5v = 0;
	avg_amp_max_12v = 0;
	avg_amp_max_5v = 0;

	last_time = TIMEus;
}

//==================================================================================================
//==================================================================================================
//==================================================================================================

void onChar(char c) {

}

