/*
 * GNU GENERAL PUBLIC LICENSE
 * Version 1, Mar 2019
 *
 * (C) 2019, Minghao Cheng <chengminghao92@gmail.com>
 */
#ifndef _LED_MATRIX_H_
#define _LED_MATRIX_H_

#include <stdint.h>
#include <signal.h> 
#include <unistd.h> 
#include <time.h>

#include "brd_config.h"
#include "./../BSP/mcp23s17.h"
#include "../util/Thread.h"

/*          Green
 *                  GPIOB.0  GPIOB.1  GPIOB.2  GPIOB.3
 *                  column0  column1  column2  column3
 * GPIOA.3  row3
 * GPIOA.2  row2
 * GPIOA.1  row1 
 * GPIOA.0  row0
 */
 
/*          Red
 *                  GPIOB.4  GPIOB.5  GPIOB.6  GPIOB.7
 *                  column0  column1  column2  column3
 * GPIOA.7  row3
 * GPIOA.6  row2
 * GPIOA.5  row1 
 * GPIOA.4  row0
 */

/*
 * Description:
 * Class of LED matrix
 */
class LED_matrix:public Thread
{
    public:
    LED_matrix(pthread_t ID);
    virtual ~LED_matrix();
    int8_t init(void);
    void write_green(uint8_t row, uint8_t val);
    void write_red(uint8_t row, uint8_t val);
    
    private:
    MCP23S17 *GPIO;
    timer_t timerid;
    static void TIMER_handler(int sig, siginfo_t *si, void *uc);
    static LED_matrix *addr;
    void refresh(void);
    void thread_handler(void);
    uint8_t v_buffer[LED_MATRIX_ROW];
};


#endif
