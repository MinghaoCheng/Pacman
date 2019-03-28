#include <stdio.h>
#include <stdint.h>

#include "Game.h"
#include "Drivers/brd_config.h"

Game game_instance(0);

static void led_refresh_handler(int val)
{
    game_instance.led_matrix_refresh();
}

int main(void)
{
    if(-1 == game_instance.init())
    {
        printf("Initialising failed\n");
        return -1;
    }
    signal(SIGALRM, led_refresh_handler);
    ualarm(LED_ROW_REFRESH_PERIOD_US, LED_ROW_REFRESH_PERIOD_US);
    game_instance.start();
    while(1)
    {
        sleep(100);
    }
}
