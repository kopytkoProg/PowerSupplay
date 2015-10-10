#ifndef __uart_h__
#define __uart_h__

#define UART_SEND_BUFFER_SIZE 		(50)

void init_uart(void);
void send(char *string);

extern void onChar(char c);

#endif
