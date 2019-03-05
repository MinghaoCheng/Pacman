#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "Drivers/mcp23s17.h"
#include "util/Thread.h"

void test_callback(uint8_t GPIO_value)
{
    printf("Callback ok\n");
}

int main(int argc, char *argv[])
{
    MCP23S17 gpio(0,0);
    printf("Initialising\n");
    gpio.init(test_callback);
    printf("Initialise finished\n");
    gpio.start();
    
    while(1)
    {
        printf("GPIO high\n");
        gpio.test_bit(1);
        sleep(1);
        printf("GPIO low\n");
        gpio.test_bit(0);
        sleep(1);
    }
    return 0;
}
