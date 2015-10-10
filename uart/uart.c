#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#define BAUD 38400
#include <util/setbaud.h>
#include <string.h>

#include "uart.h"


char usart_bufor[UART_SEND_BUFFER_SIZE + 1];
uint8_t usart_bufor_ind = 0;

enum enum_state {
	idle, sending
};
static enum enum_state volatile state = idle;

/**
 * interrupt generated after byte received.
 * received data stored in UDR
 * USART, RX Complete Handler
 */
ISR(USART_RXC_vect) {
	//	interrupt generated after byte received.
	//  received data stored in UDR
	//--------------------------------------------------
	onChar(UDR);
}

/**
 * interrupt generated when output buffer is empty
 * USART, UDR Empty Handle
 */
ISR(USART_UDRE_vect) {

	// interrupt generated when output buffer is empty
	//--------------------------------------------------
	if (usart_bufor[usart_bufor_ind] != 0) {	// check if not end of string
		UDR = usart_bufor[usart_bufor_ind++];	// send next char
	} else {									// if end of string
		state = idle;
		UCSRB &= ~(1 << UDRIE); 				// disable UDRE interrupt
	}
}

/**
 * data as a string
 */
void send(char *string) {

	/*
	 * 	The UDREn Flag indicates if the transmit buffer (UDRn) is ready to receive new data. If UDREn
	 * 	is one, the buffer is empty, and therefore ready to be written. The UDREn Flag can generate a
	 * 	Data Register Empty interrupt (see description of the UDRIE bit). UDREn is set after a reset to
	 * 	indicate that the Transmitter is ready.
	 */

	while (!(UCSRA & (1 << UDRE)) || state != idle)
		;

	state = sending;
	usart_bufor_ind = 0;						// copy data to buffer
	strcpy(usart_bufor, string);
	strcat(usart_bufor, "\r\n");
	UCSRB |= (1 << UDRIE); 						// enable empty buffer interrupt

}

void init_uart(void) {

	// set computed value by util/setbaud.h
	UBRRH = UBRRH_VALUE;
	UBRRL = UBRRL_VALUE;
#if USE_2X
	UCSRA |= (1<<U2X);
#else
	UCSRA &= ~(1 << U2X);
#endif

	//U W A G A !!!
	//W ATmega8, aby zapisać do rejestru UCSRC należy ustawiać bit URSEL
	//zobacz także: http://mikrokontrolery.blogspot.com/2011/04/avr-czyhajace-pulapki.html#avr_pulapka_2
	UCSRC =  (1<<URSEL) | (1 << UCSZ1) | (1 << UCSZ0);  		//bitów danych: 8
																//bity stopu:  1
																//parzystość:  brak

	UCSRB = (1 << TXEN) | (1 << RXEN) | (1 << RXCIE);
}

//====================================================
