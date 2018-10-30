#include "uart.h"

static char *uart_device[] = {"/dev/ttySAC0", "/dev/ttySAC1", "/dev/ttySAC2", "/dev/ttySAC3", "/dev/ttySC0", "/dev/ttySC1"};

/*!
 *<function description:>set uart as raw mode
 *@param:  uart  a pointer to UART_T type that we want to configure
 *@return: none
 */
void set_uart_raw_mode(UART_T *uart)
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


/*!
 *<function description:>uart init
 *@param:  uart      a pointer to UART_T type that we want to configure
 *@param:  dev_num   enum type ,like COM1 COM2 COM3 COM4 or OTHER
 *@param:  baud      baud rate 
 *@return: none
 */
void uart_init(UART_T *uart,  UART_NAME  dev_num, int baud)
{
	//select device name 
	#if 0
	if (dev_num <= COM4)
	{
		strcpy(uart->name, uart_device[dev_num]);
		//strcpy(uart->name, "/dev/ttySC1");
		printf("uart:uart name:%s\n", uart->name);
	}
	else if (dev_num == OTHER)
	{
		//strcpy(uart->name, uart_device[dev_num]);
		strcpy(uart->name, "/dev/ttySC11");
		printf("uart:uart name:%s\n", uart->name);	
	}
	else
	{
		printf("com port err!\n");
	}
	#endif
	strcpy(uart->name, "/dev/ttyUSB0");
	printf("uart:uart name:%s\n", uart->name);	
	//set baud
	uart->baud_rates = baud;

}

/*!
 *<function description:>uart open
 *@param:  uart      a pointer to UART_T type that we want to configure
 *@return: 0-success  -1-error
 */
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

/*!
 *<function description:>uart close
 *@param:  uart      a pointer to UART_T type that we want to configure
 *@return: none
 */
void uart_close(UART_T *uart)
{
	close(uart->fd);
	printf("uart:uart close success!\n");
}


