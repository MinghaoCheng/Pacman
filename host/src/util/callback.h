#ifndef _CALLBACK_H_
#define _CALLBACK_H_

#include <stdint.h>

class callback
{
    public:
    virtual void cb_func(uint8_t *param, uint8_t size) = 0;
};

#endif
