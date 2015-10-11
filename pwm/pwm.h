/*
 * pwm.h
 *
 *  Created on: 11 paź 2015
 *      Author: michal
 */

#ifndef PWM_PWM_H_
#define PWM_PWM_H_

void set_OCR1A(uint8_t v);
void set_TCCR1B(uint8_t v);
void init_pwm(void);


#endif /* PWM_PWM_H_ */
