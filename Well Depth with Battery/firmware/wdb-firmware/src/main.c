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
#include "rtc_sleep.h"

//USB MSC SOF Counter. I added this
volatile static uint16_t main_usb_sof_counter = 0;

int main (void)
{
	system_init();
	rtc_init();
	delay_init();
	usart_init();
	adc_init();
	
	//Configure PA9 and PA15 as outputs
	PORT->Group[0].DIRSET.reg = PORT_PA09 | PORT_PA15;
	
	while(true){
		debug_print("waking up\n\r");
		analog_on();
		led_on();
		
		
		//uhc_start();
		delay_s(2);
		
		int acc = 0;
		//Read the strain gauge ADC value and average
		for(int i = 0; i < NUMSAMPLES; i++){
			acc += adc_read();
		}
		//usb_writedata(acc);
		debug_print("Wrote Data!\n\r");
		
		analog_off();
		standby();
	}
}

void analog_on(){
	PORT->Group[0].OUTSET.reg = PORT_PA09;
}

void analog_off(){
	PORT->Group[0].OUTCLR.reg = PORT_PA15 | PORT_PA09;
}

void led_on(){
	PORT->Group[0].OUTSET.reg = PORT_PA15;
}