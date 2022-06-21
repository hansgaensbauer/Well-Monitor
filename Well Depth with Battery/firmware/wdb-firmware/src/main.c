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

int main (void)
{
	system_init();
	delay_init();
	usart_init();
	adc_init();
	
	while(true){
		delay_ms(1000);
		debug_print("Done\n\r");
	}

	/* Insert application code here, after the board has been initialized. */
}
