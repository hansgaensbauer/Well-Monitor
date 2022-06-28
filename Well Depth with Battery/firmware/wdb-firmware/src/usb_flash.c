#include "usart.h"
#include "usb_flash.h"
#include <asf.h>
#include <stdint.h>

const char firmware_filename[] = {FIRMWARE_FILE};
/* FATFS variables */
static FATFS fs;
static FIL file_object;

/*
* Test from the original example. Reads and prints first 16 chars from file.
*/
void usb_readchars(void){
	volatile uint8_t lun = LUN_ID_USB;
	memset(&fs, 0, sizeof(FATFS));
	
	FRESULT res = f_mount(lun, &fs); // Mount drive
	if (FR_INVALID_DRIVE == res) {
		debug_print("Mount Failed!\n\r");
		return;
	}
	res = f_open(&file_object,firmware_filename, FA_READ);
	if (res == FR_NOT_READY) { // LUN not ready
		debug_print("File open failed!\n\r");
		f_close(&file_object);
		return;
	}
	if (res != FR_OK) { // LUN test error
		f_close(&file_object);
		debug_print("LUN Test Error!\n\r");
		return;
	}
	
	char char_buffer[16];
	
	/* Read the first 16 bytes from USB stick into char_buffer*/
	f_read(&file_object, char_buffer, 16, NULL );
	debug_print("\n\r");
	debug_print(char_buffer);
	debug_print("\n\r");
	f_close(&file_object);
	f_mount(lun, NULL); //unmount the device
}

/*
* Saves a datapoint to the file.
*/
void usb_writedata(uint16_t data){
	volatile uint8_t lun = LUN_ID_USB;
	memset(&fs, 0, sizeof(FATFS));
	
	FRESULT res = f_mount(lun, &fs); // Mount drive
	if (FR_INVALID_DRIVE == res) {
		debug_print("Mount Failed!\n\r");
		return;
	}
	res = f_open(&file_object,firmware_filename, FA_WRITE);
	f_lseek(&file_object, file_object.fsize);  //Move to the end of the file
	if (res == FR_NOT_READY) { // LUN not ready
		debug_print("File open failed!\n\r");
		f_close(&file_object);
		return;
	}
	if (res != FR_OK) { // LUN test error
		f_close(&file_object);
		debug_print("LUN Test Error!\n\r");
		return;
	}
	
	// Print the data to the file
	char databuf[24];
	uint numchars = sprintf(databuf, "\n\rADC Value: %d", data);
	debug_print("saving data to the file: ");
	debug_print(databuf);
	debug_print("\n\r");
	f_write(&file_object, databuf, numchars, NULL);
	f_close(&file_object);
	f_mount(lun, NULL); //unmount the device
}