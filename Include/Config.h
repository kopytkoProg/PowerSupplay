#ifndef Config
#define Config

// ========== CONTROLL ==========
// PD5 : OC1A
// PD4 : OC1B

#define CONTROLL_DDR 			DDRD
#define CONTROLL_PORT 			PORTD
#define CONTROLL_PIN 			PIND

#define CONTROLL_5v_DDD 		DDD4
#define CONTROLL_12v_DDD 		DDD5

#define CONTROLL_5v_PORT 		PD4
#define CONTROLL_12v_PORT 		PD5

#define CONTROLL_5v_PIN 		PIND4
#define CONTROLL_12v_PIN 		PIND5


//========== INTERRUPT ==========
// INT0 : PD2
// INT1 : PD3

#define INT_5V_vect 			INT0_vect
#define INT_12V_vect 			INT1_vect

#define INT_DDR 				DDRD
#define INT_PORT 				PORTD
#define INT_PIN 				PIND

#define INT_5v_DDD 				DDD2
#define INT_12v_DDD 			DDD3

#define INT_5v_PORT 			PD2
#define INT_12v_PORT 			PD3

#define INT_5v_PIN 				PIND2
#define INT_12v_PIN 			PIND3


#endif

