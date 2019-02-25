#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <pthread.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <poll.h>

#include "Drivers/mcp23s17.h"
#include "Drivers/mcp23x0817.h"

int main(int argc, char *argv[])
{
    /*
    int GPIO_fd;
    char status;
    
    struct pollfd fds[1];
    int poll_ret;
    

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
    */
    uint8_t buff;
    mcp_init();
    //buff = read_test(MCP23x17_IODIRA);
    //printf("IODIRA = %x\n", buff);
    
    while(1)
    {
        printf("GPIO high\n");
        test_high();
        sleep(1);
        printf("GPIO low\n");
        test_low();
        sleep(1);
    }
    return 0;
}
