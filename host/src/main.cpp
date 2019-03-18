#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "Drivers/led_matrix.h"

uint8_t v_val[4]=
{
    0b0101,
    0b1010,
    0b0101,
    0b1010
};

LED_matrix led_matrix(1);

static void timer_handler(int val)
{
    led_matrix.refresh();
}

int main(void)
{
    
    printf("Initialising\n");
    led_matrix.init();
    printf("Initialised\n");
    signal(SIGALRM, timer_handler);
    ualarm(10,10);
    //led_matrix.start();
    while(1)
    {
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
    }
    return 0;
}
