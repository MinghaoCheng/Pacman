/*
 * GNU GENERAL PUBLIC LICENSE
 * Version 1, April 2019
 *
 * (C) 2019, Minghao Cheng <chengminghao92@gmail.com>
 */
#ifndef _GAME_H_
#define _GAME_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "util/Thread.h"
#include "util/callback.h"
#include "util/timer.h"

#include "Drivers/led_matrix.h"
#include "Drivers/ldr_matrix.h"
#include "Drivers/TCP.h"
#include "Drivers/sound.h"
#include "Drivers/Keyboard.h"

#include "Game_config.h"

struct Position
{
    int8_t row,column;
    bool Is_upon_dot;
};

class Game:public Thread, public callback, public timer
{
    public:
    Game(pthread_t ID);
    ~Game(void);
    int8_t init(void);
    void reset(void);
    
    private:
    // game logic
    bool game_running;
    int8_t game_status(void);
    uint8_t G_LED_buffer[GAME_PANEL_MATRIX_ROW];
    uint8_t LDR_buffer[GAME_PANEL_MATRIX_ROW];
    Position car_position;
    Position ghost_position;
    void ghost_position_update(void);
    // hardwares
    LED_matrix *led_matrix;
    LDR_matrix *ldr_matrix;
    Sound_dev *sound_dev;
    TCP_dev *tcp_dev;
    Keyboard *key;
    
    pthread_t id;
    void thread_handler(void);
    void cb_func(uint8_t *param, uint8_t size);
    void timerEvent(void);
    void update_LED(void);
};

#endif
