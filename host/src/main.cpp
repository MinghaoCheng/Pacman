#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "Drivers/mcp23s17.h"
#include "util/Thread.h"

void test_callback(uint8_t GPIO_value)
{
    printf("INT Callback, value = %x\n", GPIO_value);
    //if(GPIO_value & 0x01)
    //{
        //printf("INTA Callback\n");
    //}
    //if(GPIO_value & 0x02)
    //{
        //printf("INTB Callback\n");
    //}
}

int main(int argc, char *argv[])
{
    MCP23S17 gpio(0, test_callback, 0);
    printf("Initialising\n");
    gpio.init();
    printf("Initialise finished\n");
    gpio.start();
    
    while(1)
    {
        sleep(1);
    }
    return 0;
}
