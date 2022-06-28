#ifndef RTC_SLEEP_H_
#define RTC_SLEEP_H_

	#define SLEEP_PERIOD 3840
	
	//Use 32*60*180 = 345600 for 3 Hours 
	//Use 320 for 10 seconds
	//Use 3840 for 2 minutes

	//Function Prototypes
	void rtc_init(void);
	void standby(void);

#endif /* RTC_SLEEP_H_ */