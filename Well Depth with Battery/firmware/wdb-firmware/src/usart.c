/*
 * usart.c
 *
 * Created: 6/20/2022 10:28:09 AM
 *  Author: hansg
 */ 
#include "samd21.h"
#include <stdio.h>
#include <stdarg.h>

#include "usart.h"
#include "main.h"

void usart_init(){
	//Enable Advanced Peripheral Bus Clock for SERCOM0
	REG_PM_APBCMASK |= PM_APBCMASK_SERCOM0;
	
	//Set up Generic Clock Generator 0 for USART
	GCLK->CLKCTRL.reg = (
		GCLK_CLKCTRL_ID(SERCOM0_GCLK_ID_CORE) | 
		GCLK_CLKCTRL_CLKEN | 
		GCLK_CLKCTRL_GEN(0)
	);
	
	SERCOM0->USART.CTRLA.reg = ~SERCOM_USART_CTRLA_ENABLE; //disable the USART
	
	//Set up IO pins
	PORT->Group[0].DIRSET.reg = PORT_PA10;
	PORT->Group[0].DIRCLR.reg = PORT_PA11;
	PORT->Group[0].PINCFG[11].reg &= ~PORT_PINCFG_PULLEN; //enable pulldown4
	PORT->Group[0].PINCFG[10].reg |= PORT_PINCFG_PMUXEN; //Enable PMUX
	PORT->Group[0].PINCFG[11].reg |= PORT_PINCFG_PMUXEN; //Enable PMUX
	PORT->Group[0].PMUX[10>>1].bit.PMUXE = PORT_PMUX_PMUXE_C_Val; //Peripheral function C
	PORT->Group[0].PMUX[11>>1].bit.PMUXO = PORT_PMUX_PMUXO_C_Val; //Peripheral function C
	
	//Set operating mode 	
	//Set asynchronous communication mode
	//Set RXPO and TXPO
	//Set data order
	SERCOM0->USART.CTRLA.reg = (
		SERCOM_USART_CTRLA_MODE_USART_INT_CLK | 
		SERCOM_USART_CTRLA_DORD | 
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
	while(!SERCOM0->USART.INTFLAG.bit.DRE);
	SERCOM0->USART.DATA.reg = c;
}

void write_str(const char * str){
	for(int i = 0; str[i]!='\0'; i++){
		write_char(str[i]);
	}
}

void debug_print(const char *fmt, ...){
	#ifdef DEBUG_PRINT
		va_list argptr;
		char buff[MAX_PRINT_LEN];
		va_start(argptr, fmt);
		vsprintf(buff, fmt, argptr);
		va_end(argptr);
		
		write_str(buff);
	#endif //DEBUG PRING
}