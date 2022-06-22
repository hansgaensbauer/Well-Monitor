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

//Variables for MSC 
#include "string.h"
#define MAX_DRIVE _VOLUMES
#define FIRMWARE_FILE "firmware.txt"
const char firmware_filename[] = {FIRMWARE_FILE};
/* FATFS variables */
static FATFS fs;
static FIL file_object;

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
	
	///////////////////////////////////
	uhc_start();
	delay_s(2); //wait 2 seconds
	
	main_usb_sof_counter = 0;
	volatile uint8_t lun = LUN_ID_USB;
	/* Mount drive */
	memset(&fs, 0, sizeof(FATFS));
	FRESULT res = f_mount(lun, &fs);
	if (FR_INVALID_DRIVE == res) {
		debug_print("Mount Failed!\n\r");
		continue;
	}
	res = f_open(&file_object,firmware_filename, FA_READ);
	if (res == FR_NOT_READY) {
		/* LUN not ready */
		debug_print("File open failed!\n\r");
		f_close(&file_object);
		continue;
	}
	if (res != FR_OK) {
		/* LUN test error */
		f_close(&file_object);
		debug_print("File open failed!\n\r");
		continue;
	}
	/* Get size of file */
	uint32_t fw_size = f_size(&file_object);
	uint8_t char_buffer[16];
	/* Read the first 16 bytes from USB stick into char_buffer*/
	f_read(&file_object, char_buffer, 16, NULL );
	/* Clear display and print content of file */
	debug_print("\n\r");
	debug_print(char_buffer);
	f_close(&file_object);
	/////////////////////////////////
	
	while(true){
		delay_ms(1000);
		debug_print("ADC_Value: %d\n\r", adc_read());
	}

	/* Insert application code here, after the board has been initialized. */
}

//SOF Callback Function. I added this.
void main_usb_sof_event(void)
{
	main_usb_sof_counter++;
}