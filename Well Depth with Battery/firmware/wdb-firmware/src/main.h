/*
 * main.h
 *
 * Created: 6/20/2022 1:57:05 PM
 *  Author: hansg
 */ 


#ifndef MAIN_H_
#define MAIN_H_

	#define DEBUG_PRINT
	
	#define NUMSAMPLES 8 //number of samples at each measurement for oversampling

	//Function prototypes
	void analog_on(void);
	void analog_off(void);
	void led_on(void);

#endif /* MAIN_H_ */