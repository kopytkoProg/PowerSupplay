/*
 * pwm.h
 *
 *  Created on: 11 paź 2015
 *      Author: michal
 */

#ifndef PWM_PWM_H_
#define PWM_PWM_H_

void set_OCR1A(uint16_t v);
void set_TCCR1B(uint16_t v);
void init_pwm(void);
void duty_OCR1A(float duty);
void duty_OCR1B(float duty);

#endif /* PWM_PWM_H_ */
