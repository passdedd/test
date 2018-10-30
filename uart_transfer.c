#include "uart.h"
#define UART_RX_FLAG 0
#define UART_TX_FLAG 1
#define MAX_BUFFER_SIZE 1024
void data_transfer(int fd, int rx_flag, int size, char *buf)
{

        if (UART_RX_FLAG == rx_flag)
                return read(fd, buf, size);
        else if (UART_TX_FLAG == rx_flag)
                return write(fd, buf, size);
        else {
                printf("error rx_flag is error\n");
                return -1;
        }
}








