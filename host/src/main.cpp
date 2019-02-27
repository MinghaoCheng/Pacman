#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "Drivers/mcp23s17.h"
#include "util/Thread.h"

void test_callback(uint8_t GPIO_value)
{
    printf("Callback ok\n");
}

void test_thread_func(void)
{
    MCP23S17 *gpio = new MCP23S17(0);
    gpio->init(test_callback);
    while(1)
    {
        printf("GPIO high\n");
        gpio->test_bit(1);
        sleep(1);
        printf("GPIO low\n");
        gpio->test_bit(0);
        sleep(1);
    }
}

void test_GPIO_func(void)
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
}

int main(int argc, char *argv[])
{
    Thread gpio_expander_test_thread(test_thread_func, 0);
    gpio_expander_test_thread.start();
    gpio_expander_test_thread.join();

    return 0;
}
