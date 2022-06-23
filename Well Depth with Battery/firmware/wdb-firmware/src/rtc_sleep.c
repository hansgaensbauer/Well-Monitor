 
#include "samd21.h"
#include "usart.h"
#include "rtc_sleep.h"

void rtc_init(){
	//Enable Advanced Peripheral Bus Clock for RTC
	REG_PM_APBAMASK |= PM_APBAMASK_RTC;
	
	//Set up Generic Clock Generator 2
	GCLK->GENCTRL.reg = (
		GCLK_GENCTRL_GENEN |
		GCLK_GENCTRL_SRC(GCLK_GENCTRL_SRC_XOSC32K_Val) | 
		GCLK_GENCTRL_ID(2)
	);
	
	while(GCLK->STATUS.bit.SYNCBUSY); //wait for sync
		
	//Select Generic Clock Generator 2 for RTC
	GCLK->CLKCTRL.reg = (
		GCLK_CLKCTRL_ID(RTC_GCLK_ID) |
		GCLK_CLKCTRL_CLKEN |
		GCLK_CLKCTRL_GEN(2)
	);
	
	while(GCLK->STATUS.bit.SYNCBUSY); //wait for sync
	RTC->MODE1.CTRL.bit.ENABLE = 0; //Disable the RTC
	
	//Select operating mode 1 and 1024 as the prescaler (32 counts/second)
	RTC->MODE1.CTRL.reg = (
		RTC_MODE1_CTRL_MODE(RTC_MODE1_CTRL_MODE_COUNT16_Val) | 
		RTC_MODE1_CTRL_PRESCALER(RTC_MODE1_CTRL_PRESCALER_DIV1024)
	);
	
	RTC->MODE1.PER.reg = 320; //Set the top of the counter to 10 seconds (320)
	RTC->MODE1.INTENSET.bit.OVF = 1; //Enable the overflow interrupt
	
	NVIC_SetPriority(RTC_IRQn, 3); //Set RTC IRQ Priority to 3
	NVIC_EnableIRQ(RTC_IRQn); //Enable the interrupt
	
	RTC->MODE1.CTRL.bit.ENABLE = 1; //Start the RTC
	
	__enable_irq(); //Enable global interrupts
	
}

void RTC_Handler(void){

	// Interrupt processing code
	PORT->Group[0].OUTTGL.reg = PORT_PA15;
	//debug_print("fire\n\r");
	
	// Acknowledge the interrupt (clear MC0 interrupt flag to re-arm)
	RTC->MODE1.INTFLAG.reg |= RTC_MODE1_INTFLAG_OVF;

}