#ifndef Config
#define Config

//========== CONTROLL ===========
// PD5 : OC1A
// PD4 : OC1B

//========== LCD ================
#define LCD_FORMAT 				("%5.2f %4.2f %4.2f")

//========== MATH ===============
// Internal Voltage Reference  min 2.3  typ 2.56  max 2.7
#define VREF 					(2.51)

#define SCALE_12V 				(1.0 / (1.0 + 4.7))
#define SCALE_5V 				(1.0 / (1.0 + 1.0))

#define SCALE_OPAMP				(3.3 / 2.2)

// TODO: Change "0.39 / 1.0" to "0.39 / 2.0"
#define RESISTOR				(0.39 / 2.0)

//========== INTERRUPT ==========
// INT0 : PD2
// INT1 : PD3

//========== ADC ================

#define VOLTAGE_V5			(4)
#define CURRENT_V5			(0)
#define MAX_CURRENT_V5		(2)

#define VOLTAGE_V12			(5)
#define CURRENT_V12			(1)
#define MAX_CURRENT_V12		(3)

#endif

