/*
 * GNU GENERAL PUBLIC LICENSE
 * Version 1, Mar 2019
 *
 * (C) 2019, Minghao Cheng <chengminghao92@gmail.com>
 */
#include <stdio.h>
#include <stdint.h>

#include "Game.h"
int main(void)
{
    Game game_instance(0);
    if(-1 == game_instance.init())
    {
        printf("Initialising failed\n");
        return -1;
    }
    game_instance.thread_start();
    while(1)
    {
        sleep(100);
    }
}
