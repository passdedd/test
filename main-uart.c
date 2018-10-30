#include "uart.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h> 



int main(int argc, char **argv)
{

	int retval = -1;
	int i;
        UART_T myuart;
        FILE *f;
        char s[1024];
        int ret;
        char rbuf[1024];
        int timecount;
        int maxfd;
        int count;
        int fdarr[2] = { 0 };
        fd_set rfds;//设置监听读集合
        fd_set wfds;//设置监听读集合
        struct timeval tv;//设置等待时间，0不等待，NULL一直等待。
        uart_init(&myuart,"/dev/ttyUSB0", B115200);
	retval = uart_open(&myuart);
        maxfd = myuart.fd +1;
        FD_ZERO(&rfds);//清空集合
        FD_ZERO(&wfds);//清空集合
        FD_SET(myuart.fd, &rfds);/*add uart fd to select set*/
        FD_SET(myuart.fd, &wfds);/*add uart fd to select set*/
        int read_len= 0;
        char read_buf[20];
        FILE *f;
        char s[1024];
        int ret;
        f = popen("./test.py 99", "r");
        if (retval == -1)
                return -1;
        ret = pipe(fdarr);
        if (ret == -1)
        {
                printf("pipe() is failed ! message :%s\n", strerror(errno));
                return -1;
        }
        pid_t child = fork();
        if (child == -1)
        {
                printf("system is game over !\n");
                return -1;
        }
        if (child == 0){
                close(fdarr[0]);
                while((ret=fread(s,3,1024,f))>0) {
                        fwrite(s,3,ret,fdarr[1]);
                }
                fclose(f);
                return -1;
        } else {
                close(fdarr[1]);
                while (1) {
                        tv.tv_sec = 10;
                        tv.tv_usec = 0;//设置等待时间
                        ret = select(maxfd, &rfds, &wfds, NULL, &tv);
                        if (ret < 0){
                                printf("err: %s,%d\n",__func__,__LINE__);
                        } else if (FD_ISSET(myuart.fd, &rfds)){
                                memset(rbuf, 0x00, sizeof(rbuf));
                                count = read(myuart.fd, rbuf, sizeof(rbuf));
                                /*处理数据*/
                                printf("rev uart data count = %d ,%s,%d\n",__func__,__LINE__,count);
                        } else if (FD_ISSET(myuart.fd, &wfds)){


                        }
                }                                                                                   


        }

        set_uart_mode(&myuart);
        while (1) {
                tv.tv_sec = 10;
                tv.tv_usec = 0;//设置等待时间
                ret = select(maxfd, &rfds, &wfds, NULL, &tv);
                if (ret < 0){
                        printf("err: %s,%d\n",__func__,__LINE__);
                } else if (FD_ISSET(myuart.fd, &rfds)){
                        memset(rbuf, 0x00, sizeof(rbuf));
                        count = read(myuart.fd, rbuf, sizeof(rbuf));
                        /*处理数据*/
                        printf("rev uart data count = %d ,%s,%d\n",__func__,__LINE__,count);
                } else if (FD_ISSET(myuart.fd, &wfds)){


                }
        }

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
