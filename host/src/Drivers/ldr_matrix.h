/*
 * GNU GENERAL PUBLIC LICENSE
 * Version 1, Mar 2019
 *
 * (C) 2019, Minghao Cheng <chengminghao92@gmail.com>
 */
#ifndef _LDR_MATRIX_
#define _LDR_MATRIX_

#include <stdint.h>

#include "./../Drivers/mcp23s17.h"
#include "./../util/Thread.h"
#include "./../util/callback.h"
#include "brd_config.h"

/*      column0     column1     column2     column3
 * row0 GPIOA.0x01  GPIOA.0x02  GPIOA.0x04  GPIOA.0x08
 * row1 GPIOA.0x10  GPIOA.0x20  GPIOA.0x40  GPIOA.0x80
 * row2 GPIOB.0x01  GPIOB.0x02  GPIOB.0x04  GPIOB.0x08
 * row3 GPIOB.0x10  GPIOB.0x20  GPIOB.0x40  GPIOB.0x80
 */

/*
 * Description:
 * Class of Light-Dependent-Resistor matrix
 */
class LDR_matrix: public callback
{
    public:
    LDR_matrix(pthread_t ID, callback *INT_callback);
    virtual ~LDR_matrix(void);
    int8_t init(void);
    
    private:
    pthread_t id;
    MCP23S17 *GPIO;
    virtual void cb_func(uint8_t *param, uint8_t size);
    callback *INT_handler;
};

#endif
