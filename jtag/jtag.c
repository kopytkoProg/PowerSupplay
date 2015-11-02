#include <avr/io.h>

// DISABLE JTAG
void JTAG_is_IO(void) {
	MCUCSR = 0x80;
	MCUCSR = 0x80;
}

// ENABLE JTAG
void JTAG_is_JTAG(void) {
	MCUCSR = 0x00;
	MCUCSR = 0x00;
}
