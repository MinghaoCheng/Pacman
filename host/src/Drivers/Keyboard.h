/*
 * GNU GENERAL PUBLIC LICENSE
 * Version 1, Mar 2019
 *
 * (C) 2019, Minghao Cheng <chengminghao92@gmail.com>
 */
 
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <stdint.h>
#include <stdio.h>

#include <fcntl.h>
#include <unistd.h>
#include <poll.h>

#include "brd_config.h"
#include "../util/Thread.h"
#include "../util/callback.h"

/*
 * Description:
 * Class, driver of the keyboard
 */
class Keyboard:public Thread
{
    public:
    Keyboard(pthread_t ID, callback *CB);
    virtual ~Keyboard();
    int8_t init(void);
    private:
    callback *cb;
    void thread_handler(void);
    
    struct pollfd fds;
};

#endif
