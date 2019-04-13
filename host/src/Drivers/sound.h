/*
 * GNU GENERAL PUBLIC LICENSE
 * Version 1, Mar 2019
 *
 * (C) 2019, Minghao Cheng <chengminghao92@gmail.com>
 */
#ifndef _SOUND_H_
#define _SOUND_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

class Sound_dev
{
    public:
    Sound_dev();
    virtual ~Sound_dev();
    
    int8_t init(void);
    void play_wav(const char *file_name);
};

#endif
