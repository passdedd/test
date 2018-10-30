#include "uart.h"


#if 1
int main(int argc, char **argv)
{

	int retval = -1;
	int i;
	UART_T myuart;
	printf("hello\n");
	uart_init(&myuart,COM2, B115200);
	retval = uart_open(&myuart);
        int read_len= 0;
        char read_buf[20];
        if (retval == -1)
		return -1;
	set_uart_raw_mode(&myuart);
        /*
	while(1)
	{*/	
		for (i=0; i<10; i++)
		{
			retval = write(myuart.fd, (char*)&i, sizeof(i));
			printf("write 0x%xh\n", i);
			sleep(1);
		}
/*	}
*/
        tcflush(myuart.fd, TCIOFLUSH);
        fflush(NULL);
        while(1)
        {
                memset(read_buf,0x0, sizeof(read_buf));
        /*        while((read_len = read(uart.fd, &data, sizeof(data)) > 0))
                {
                     tcflush(uart.fd, TCOFLUSH);
                     printf("we get data is = %d\n", data);
                }

                read(uart.fd, &data, sizeof(data));
                printf("we get %d\n", data);
                                      fflush(NULL);*/
                read_len = read(myuart.fd, read_buf, sizeof(read_buf));
                printf("receive size = %d\n", read_len);
                for (i=0; i<read_len; i++)
                {
                        printf("read_buf[%d]=0x%xh\n", i, read_buf[i]);
                        fflush(NULL);
                }
                //printf("read_buf=%c\n", read_buf);
        }                                                              
        

	uart_close(&myuart);
	return 0;
}
#else
int main(int argc, char **argv)
{

	char retval = -1;
	int i;
	UART_T myuart;
	char *msg = "hello";
	pid_t pid;

	uart_init(&myuart,11, B115200);
	retval = uart_open(&myuart);
	if (retval == -1)
		return -1;
	set_uart_raw_mode(&myuart);
#if 1
	pid = fork();
	if (pid < 0)
	{
	}
	else if (pid == 0)
	{
		
		while(1)
		{
			#if 1	
			int write_len = 0;
			char write_buf[20];
			bzero(write_buf, sizeof(write_buf));
	        fgets(write_buf, sizeof(write_buf), stdin);
			//write_buf[strlen(write_buf)] = '\0';

				char cmd[] = {0x3C, 0x07, 0x12, 0x08, 0x10, 0x03, 0x8F};	
				retval = write(myuart.fd, cmd, strlen(cmd));
				if (retval > 0)
					printf("cmd send ok\n");
/*
			if (strcmp(write_buf, "start") == 0){
				char cmd[] = {0x3C, 0x07, 0x12, 0x08, 0x10, 0x03, 0x8F};	
				retval = write(myuart.fd, cmd, strlen(cmd));
				if (retval > 0)
					printf("cmd send ok\n");
			}
			else{
				retval = write(myuart.fd, write_buf, strlen(write_buf));
				printf("we input %s\n", write_buf);
			}
*/
			#else	
			char tmp[]={0x01, 0x03, 0x0c, 0x00};
			retval = write(myuart.fd, tmp, strlen(tmp));
			if (retval > 0)
				printf("send ok\n");
			sleep(2);
			#endif
		}
	}
	else
	{
		//tcflush(myuart.fd, TCIOFLUSH);
		//fflush(NULL);
		while(1)
		{
			int read_len= 0;
			char read_buf[20];
			bzero(read_buf, sizeof(read_buf));
	       // while((read_len = read(uart.fd, &data, sizeof(data)) > 0))
	       // {
	        //	tcflush(uart.fd, TCOFLUSH);
	        //	printf("we get data is = %d\n", data);
	       // }
	       
	        //read(uart.fd, &data, sizeof(data));
	        //printf("we get %d\n", data);
	        //			fflush(NULL);
			read_len = read(myuart.fd, read_buf, sizeof(read_buf));
		   	printf("receive size = %d\n", read_len);
			for (i=0; i<read_len; i++)
			{
				printf("read_buf[%d]=0x%xh\n", i, read_buf[i]);
				//fflush(NULL);
			}
			//printf("read_buf=%c\n", read_buf);
		}		

	}
#endif
	uart_close(&myuart);
	return 0;
}
#endif








