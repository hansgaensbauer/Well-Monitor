/*
 * usart.h
 *
 * Created: 6/20/2022 10:28:26 AM
 *  Author: hansg
 */ 


#ifndef USART_H_
#define USART_H_

	#define F_CPU 8000000UL
	#define TX_PIN (1 << 10) //PA10

	//Function declarations
	void init_usart(void);




#endif /* USART_H_ */