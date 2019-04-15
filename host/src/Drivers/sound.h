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

#include "./../util/Thread.h"

/**
 * Class handling the sound
 */
class Sound_dev:public Thread
{
    public:
    Sound_dev(pthread_t ID);
    virtual ~Sound_dev();
    
    int8_t init(void);
    void play_wav(const char *File_name);
    void play_wav_blocking(const char *File_name);
    
    
    private:
    void thread_handler(void);
    char *file_name;
    bool flag;
};

#endif
