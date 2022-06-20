/*
 * usart.c
 *
 * Created: 6/20/2022 10:28:09 AM
 *  Author: hansg
 */ 

#include "usart.h"
#include "samd21.h"

void usart_init(){
	SERCOM0->USART.CTRLA.reg = ~SERCOM_USART_CTRLA_ENABLE; //disable the USART
	
	//Set up IO pins
	REG_PORT_DIRSET0 |= TX_PIN;
	PORT->Group[0].PINCFG[10].reg |= PORT_PINCFG_PMUXEN; //Enable PMUX
	PORT->Group[0].PINCFG[11].reg |= PORT_PINCFG_PMUXEN; //Enable PMUX
	PORT->Group[0].PMUX[10/2].bit.PMUXE = PORT_PMUX_PMUXE_C_Val; //Peripheral function C
	PORT->Group[0].PMUX[11/2].bit.PMUXO = PORT_PMUX_PMUXO_C_Val; //Peripheral function C
	
	//Set operating mode 	
	//Set asynchronous communication mode
	//Set RXPO and TXPO
	//Set data order
	SERCOM0->USART.CTRLA.reg = (
		SERCOM_USART_CTRLA_MODE_USART_INT_CLK | 
		SERCOM_USART_CTRLA_RXPO(0x3) | 
		SERCOM_USART_CTRLA_TXPO(0x1)
		);
		
	//enable the transmitter with rxen and txen
	SERCOM0->USART.CTRLB.reg = (
		SERCOM_USART_CTRLB_RXEN | 
		SERCOM_USART_CTRLB_TXEN | 
		SERCOM_USART_CTRLB_CHSIZE(0) //8 data bits
		);
	
	uint64_t baudRate = (uint64_t)65536 * (F_CPU - 16 * BAUDRATE) / F_CPU;
	SERCOM0->USART.BAUD.reg = (uint32_t)baudRate; //write baud register
	
	//Enable the USART
	SERCOM0->USART.CTRLA.reg |= SERCOM_USART_CTRLA_ENABLE;
}

void write_char(char c){
	/*
	while(!(SERCOM0->USART.INTFLAG.bit.TXC)){
	}
	SERCOM0->USART.DATA.reg = c;
	*/
}