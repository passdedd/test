#ifndef __UART_H__
#define __UART_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h>
#include <string.h>
#include <termios.h>

#define MAX_DATA 100

typedef struct
{
	int fd;
	char name[30];
	struct termios options;
	int baud_rates;	
}UART_T;



typedef enum 
{
	RAW = 0,
}UART_MODE;

typedef enum
{
	COM1 = 0,
	COM2 = 1,
	COM3 = 2,
	COM4 = 3,
	OTHER = 4,
}UART_NAME;








extern int uart_open(UART_T *uart);
extern void uart_close(UART_T *uart);
extern void uart_init(UART_T *uart,  UART_NAME  dev_num, int baud);
extern void set_uart_raw_mode(UART_T *uart);



#endif

