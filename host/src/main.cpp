#include <stdio.h>
#include <stdint.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <poll.h>

#include "Drivers/spi.h"

int main(void)
{
    int GPIO_fd;
    char status;
    
    struct pollfd fds[1];
    int poll_ret;
    
    SPI::init();

    GPIO_fd = open("/dev/GPIO_INT", O_RDWR,S_IRUSR | S_IWUSR);
    if(GPIO_fd == -1)
    {
        printf("file open failed\n");
    }
    
    fds[0].fd = GPIO_fd;
    fds[0].events = POLLPRI;
    
    while(1)
    {
        poll_ret = poll(fds, 1, -1);
        printf("POLL triggered  %d\n", poll_ret);
        if(poll_ret != 0)
        {
            status = read(fds[0].fd, &status, 1);
            if(status == 1)
            {
                printf("GPIO int!\n");
            }
        }
    }
    
    return 0;
}
