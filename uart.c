#include "uart.h"

void set_uart_mode(UART_T *uart)
{
	int retval = -1;
	
	retval = tcgetattr(uart->fd, &uart->options);	//get default attribute
	if (retval != 0)
		printf("uart:uart setup err!");

	if(fcntl( uart->fd,F_SETFL,0)<0)                //blocking 
	{
		printf("fcntl F_SETFL\n");
	}
	//configure uart as raw mode, 8 N 1
	uart->options.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
	uart->options.c_oflag &= ~OPOST;
	uart->options.c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
	uart->options.c_cflag &= ~(CSIZE|PARENB);
	uart->options.c_cflag |=  CS8;
	uart->options.c_cflag |= CRTSCTS;            //no flow cont
	//uart->options.c_cc[VTIME] = 0;
	//uart->options.c_cc[VMIN]  = 5;
	//set baud rate
        cfsetospeed(&uart->options,uart->baud_rates);        
        cfsetispeed(&uart->options,uart->baud_rates); 
	tcflush(uart->fd, TCIOFLUSH); 					//flush               
	tcsetattr(uart->fd,TCSANOW,&uart->options);		//make change immediately
	tcflush(uart->fd, TCIOFLUSH);  
}

void uart_init(UART_T *uart, char *name, int baud)
{
	strcpy(uart->name, name);
	printf("uart:uart name:%s\n", uart->name);	
	uart->baud_rates = baud;

}

int uart_open(UART_T *uart)
{
	uart->fd = open(uart->name, O_RDWR|O_NOCTTY|O_NDELAY);
	if(uart->fd < 0)
	{
		printf("uart:uart open err!\n");
		return -1;
	}
	printf("uart:uart open success!\n");
	return 0;
}

void uart_close(UART_T *uart)
{
	close(uart->fd);
	printf("uart:uart close success!\n");
}


