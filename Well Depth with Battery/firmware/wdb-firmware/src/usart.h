#ifndef USART_H_
#define USART_H_
	
	#define F_CPU 48000000UL
	#define BAUDRATE 9600
	#define TX_PIN (1 << 10) //PA10
	#define MAX_PRINT_LEN 120

	//Function declarations
	void usart_init(void);
	void write_char(char);
	void write_str(const char *);
	void debug_print(const char *, ...) __attribute__ ((format (gnu_printf, 1, 2)));

#endif /* USART_H_ */