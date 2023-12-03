#ifndef USB_FLASH_H_
#define USB_FLASH_H_

	#include "string.h"
	#include <stdint.h>
	
	#define MAX_DRIVE _VOLUMES
	#define FIRMWARE_FILE "welldata.txt"
	
	void usb_readchars(void);
	void usb_writedata(uint16_t, int);


#endif /* USB_FLASH_H_ */