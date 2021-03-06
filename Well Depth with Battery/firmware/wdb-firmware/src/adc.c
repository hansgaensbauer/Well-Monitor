/*
 * adc.c
 *
 * Created: 6/20/2022 9:39:43 PM
 *  Author: hansg
 */ 
#include <stdint.h>
#include "samd21.h"

#include "adc.h"
#include "usart.h"


void adc_init(){
	debug_print("interrupt\n\r");
	//Enable Advanced Peripheral Bus Clock for ADC
	PM->APBCMASK.reg |= PM_APBCMASK_ADC;
	
	//Set up Generic Clock Generator 1 for ADC
	GCLK->CLKCTRL.reg = (
		GCLK_CLKCTRL_ID(ADC_GCLK_ID) |
		GCLK_CLKCTRL_CLKEN |
		GCLK_CLKCTRL_GEN(1)
	);
	
	while(GCLK->STATUS.bit.SYNCBUSY); //wait for bus sync
	
	//See https://blog.thea.codes/reading-analog-values-with-the-samd-adc/ . This code loads an ADC calibration
	uint32_t bias = (*((uint32_t *) ADC_FUSES_BIASCAL_ADDR) & ADC_FUSES_BIASCAL_Msk) >> ADC_FUSES_BIASCAL_Pos;
	uint32_t linearity = (*((uint32_t *) ADC_FUSES_LINEARITY_0_ADDR) & ADC_FUSES_LINEARITY_0_Msk) >> ADC_FUSES_LINEARITY_0_Pos;
	linearity |= ((*((uint32_t *) ADC_FUSES_LINEARITY_1_ADDR) & ADC_FUSES_LINEARITY_1_Msk) >> ADC_FUSES_LINEARITY_1_Pos) << 5;
	while (ADC->STATUS.bit.SYNCBUSY);   // Wait for bus synchronization.
	ADC->CALIB.reg = ADC_CALIB_BIAS_CAL(bias) | ADC_CALIB_LINEARITY_CAL(linearity); // Write the calibration data.
	while (ADC->STATUS.bit.SYNCBUSY);  // Wait for bus synchronization.

	ADC->REFCTRL.reg = ADC_REFCTRL_REFSEL_INTVCC1; //Internal Ref
	ADC->AVGCTRL.reg = ADC_AVGCTRL_SAMPLENUM_1; // Only capture one sample 
	
	//Set the clock prescaler to 512, resolution to 12bit.
	ADC->CTRLB.reg = ADC_CTRLB_PRESCALER_DIV4 |
					 ADC_CTRLB_RESSEL_12BIT;

	// Configure the input parameters.
	ADC->INPUTCTRL.reg = ADC_INPUTCTRL_GAIN_DIV2 |
						 ADC_INPUTCTRL_MUXNEG_GND |
						 ADC_INPUTCTRL_MUXPOS_PIN0;
						 
	PORT->Group[0].DIRCLR.reg = PORT_PA02; //PA02/AIN[0] as input
	PORT->Group[0].PINCFG[2].reg |= PORT_PINCFG_PMUXEN; //Peripheral mux for PA02
	PORT->Group[0].PMUX[2>>1].bit.PMUXE = PORT_PMUX_PMUXE_B_Val;
	while (ADC->STATUS.bit.SYNCBUSY);  // Wait for bus synchronization.
	
	ADC->CTRLA.reg |= ADC_CTRLA_ENABLE; //Enable ADCs
}

uint16_t adc_read(){
	debug_print("\tReading ADC\n\r");
	while (ADC->STATUS.bit.SYNCBUSY); // Wait for bus synchronization.
	
	ADC->SWTRIG.reg |= ADC_SWTRIG_START; // Start the ADC using a software trigger.
	while(!ADC->INTFLAG.bit.RESRDY); // Wait for the result ready flag to be set.
	ADC->INTFLAG.reg = ADC_INTFLAG_RESRDY; //clear the flag
	return (uint16_t) ADC->RESULT.reg;
}
