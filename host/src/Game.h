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
#include "util/Thread.h"
#include "util/callback.h"
#include "Drivers/led_matrix.h"
#include "Drivers/ldr_matrix.h"
#include "Drivers/brd_config.h"

struct Car_position
{
    uint8_t row,column;
    bool Is_upon_dot;
};

class Game:public Thread, public callback
{
    public:
    Game(pthread_t ID);
    ~Game(void);
    uint8_t init(void);
    void reset(void);
    
    private:
    // game logic
    int8_t game_status(void);
    uint8_t v_buffer[LED_MATRIX_ROW][3];        //[:][0]-->Green, [:][1]-->Red,[:][2]-->Values of LDR matrix
    Car_position car_position;
    void monster_update(void);
    // hardwares
    LED_matrix *led_matrix;
    LDR_matrix *ldr_matrix;
    //Sound_dev *sound_dev;
    //Keyboard *key;
    
    pthread_t id;
    virtual void run(void);
    virtual void cb_func(uint8_t *param, uint8_t size);
};

#endif
