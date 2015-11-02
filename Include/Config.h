#ifndef Config
#define Config

//========== CONTROLL ===========
// PD5 : OC1A
// PD4 : OC1B



//========== LCD ================
#define LCD_FORMAT 				(" %4.1f %4.1f %4.1f")

//========== MATH ===============
// Internal Voltage Reference  min 2.3  typ 2.56  max 2.7
#define VREF 					(2.51)
#define SCALE_12V 				(1.0/3.0)
#define SCALE_5V 				(4.0/5.0)



//========== INTERRUPT ==========
// INT0 : PD2
// INT1 : PD3

//========== ADC ================



#define VOLTAGE_V5			(0)
#define CURRENT_V5			(2)
#define MAX_CURRENT_V5		(4)

#define VOLTAGE_V12			(1)
#define CURRENT_V12			(3)
#define MAX_CURRENT_V12		(5)

#endif

