/*
 * adc.h
 *
 *  Created on: 10 paź 2015
 *      Author: michal
 */

#ifndef ADC_ADC_H_
#define ADC_ADC_H_


#define ADC_CHANELS			(6)
uint16_t adc_value[ADC_CHANELS];



void init_adc(void);
//uint16_t adc_read(uint8_t ch);
void start_all_chanel_converrsion(void);
uint8_t is_ready(void);





#endif /* ADC_ADC_H_ */
