#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "Drivers/led_matrix.h"
uint8_t v_val[4]=
{
    0b1111,
    0b1111,
    0b1111,
    0b1111
};


LED_matrix led_matrix(0);
static void timer_handler(int val)
{
    led_matrix.refresh();
}
int main(void)
{
    
    MCP23S17 LDR_matrix(0,1);
    printf("Initialising\n");
    if(-1 != LDR_matrix.init())
    {
        printf("LDR matrix initialised\n");
    }
    else
    {
        printf("LDR matrix initialising failed\n");
    }
    if(-1 != led_matrix.init())
    {
        printf("LED matrix initialised\n");
    }
    else
    {
        printf("LED matrix initialising failed\n");
    }
    printf("Initialised\n");
    signal(SIGALRM, timer_handler);
    ualarm(250,250);
    //led_matrix.start();
    led_matrix.write_val(v_val);
    while(1)
    {
        /*
        led_matrix.write_val(v_val);
        sleep(1);
        for (uint8_t i=0; i<4; i++)
        {
            v_val[i] = (~v_val[i])&0x0f;
        }
        led_matrix.write_val(v_val);
        sleep(1);
        for (uint8_t i=0; i<4; i++)
        {
            v_val[i] = (~v_val[i])&0x0f;
        }
        * */
    }
    return 0;
}
