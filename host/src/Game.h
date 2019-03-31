#ifndef _GAME_H_
#define _GAME_H_

#include <stdint.h>
#include <stdio.h>
#include "util/Thread.h"
#include "util/callback.h"
#include "Drivers/led_matrix.h"
#include "Drivers/ldr_matrix.h"
#include "Drivers/brd_config.h"

class Game:public Thread, public callback
{
    public:
    Game(pthread_t ID);
    ~Game(void);
    uint8_t init(void);
    void led_matrix_refresh(void);
    void reset(void);
    
    private:
    pthread_t id;
    uint8_t vram[LED_MATRIX_ROW];
    LED_matrix *led_matrix;
    LDR_matrix *ldr_matrix;
    virtual void run(void);
    virtual void cb_func(uint8_t *param, uint8_t size);
    
    //Sound_dev *sound_dev;
    //Keyboard *key;
};

#endif
