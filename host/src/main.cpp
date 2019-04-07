/*
 * GNU GENERAL PUBLIC LICENSE
 * Version 1, Mar 2019
 *
 * (C) 2019, Minghao Cheng <chengminghao92@gmail.com>
 */
#include <stdio.h>
#include <stdint.h>

#include "Game.h"
#include "Drivers/brd_config.h"
#include "Drivers/sound.h"

int main(void)
{
    Game game_instance(0);
    if(-1 == game_instance.init())
    {
        printf("Initialising failed\n");
        return -1;
    }
    //signal(SIGALRM, led_refresh_handler);
    //ualarm(LED_ROW_REFRESH_PERIOD_US, LED_ROW_REFRESH_PERIOD_US);
    game_instance.start();
    
    //play_sound("/home/pi/Desktop/Pacman/host/1.wav");
    while(1)
    {
        sleep(100);
    }
}
