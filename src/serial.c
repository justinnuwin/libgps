#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <inttypes.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/ioctl.h>

#include "serial.h"

int uart0_filestream = -1;

void serial_init(void)
{
    uart0_filestream = open(PORTNAME, O_RDWR | O_NOCTTY | O_NDELAY);

    if (uart0_filestream == -1)
    {
        //TODO error handling...
    }
}

void serial_config(void)
{
    struct termios options;
    tcgetattr(uart0_filestream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = ICANON;
    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);
}

void serial_println(const char *line, int len)
{
    if (uart0_filestream != -1) {
        char *cpstr = (char *)malloc((len+1) * sizeof(char));
        strcpy(cpstr, line);
        cpstr[len-1] = '\r';
        cpstr[len] = '\n';

        int count = write(uart0_filestream, cpstr, len+1);
        if (count < 0) {
            //TODO: handle errors...
        }
        free(cpstr);
    }
}

// Read a line from UART.
int serial_readln(char *buffer)
{
    int bytes_read = 0;
    struct timeval timeout = {.tv_sec = 0, .tv_usec = 0};
    fd_set input_fdset;
    FD_ZERO(&input_fdset);
    FD_SET(uart0_filestream, &input_fdset);

    switch (select(uart0_filestream + 1, &input_fdset, NULL, NULL, &timeout)) {
        case -1:
            perror("Select failed");
            break;
        case 0:
            // Nothing available to read
            break;
        default: {
            ioctl(uart0_filestream, FIONREAD, &bytes_read);
            int bytes_recv = bytes_read;
            while (bytes_recv > 0) {
                if (read(uart0_filestream, (void *)(buffer), 1) < 0)
                    perror("Error reading UART buffer");
                if (*buffer == '\n') {
                    *buffer++ = '\0';
                    break;
                }
                buffer++;
                bytes_recv--;
            }
            break;
        }
    };
    return bytes_read;
}

void serial_close(void)
{
    close(uart0_filestream);
}

