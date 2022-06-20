#ifndef USART_H_
#define USART_H_

	#include <stdbool.h>

	#define BAUDRATE 9600
	#define F_CPU 8000000UL
	#define TX_PIN (1 << 10) //PA10

	//Function declarations
	void usart_init(void);
	void write_char(char);

#endif /* USART_H_ */