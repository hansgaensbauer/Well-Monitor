/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to system_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#include <asf.h>
#include <string.h>

#include "main.h"
#include "usart.h"
#include "adc.h"
#include "usb_flash.h"

//USB MSC SOF Counter. I added this
volatile static uint16_t main_usb_sof_counter = 0;

int main (void)
{
	system_init();
	delay_init();
	usart_init();
	adc_init();
	
	//Turn on the analog subsystem
	PORT->Group[0].DIRSET.reg = PORT_PA09;
	PORT->Group[0].OUTSET.reg = PORT_PA09;
	
	uhc_start();
	delay_s(2); //wait 2 seconds
	
	usb_readchars();
	
	while(true){
		delay_ms(1000);
		debug_print("ADC_Value: %d\n\r", adc_read());
	}

	/* Insert application code here, after the board has been initialized. */
}