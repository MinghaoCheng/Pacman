/*
 * GNU GENERAL PUBLIC LICENSE
 * Version 1, Mar 2019
 *
 * (C) 2019, Minghao Cheng <chengminghao92@gmail.com>
 */
#ifndef _CALLBACK_H_
#define _CALLBACK_H_

#include <stdint.h>

/**
 * Callback base function
 */
class callback
{
    public:
    virtual void cb_func(uint8_t *param, uint8_t size) = 0;
};

#endif
