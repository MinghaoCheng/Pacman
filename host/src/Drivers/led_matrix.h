#ifndef _LED_MATRIX_H_
#define _LED_MATRIX_H_

#include <stdint.h>
#include <signal.h> 
#include <unistd.h> 
#include <time.h>

#include "brd_config.h"
#include "mcp23s17.h"
#include "../util/Thread.h"

/*      column0  column1  column2  column3
 * row0
 * rOW1
 * row2
 * row3
 */

class LED_matrix:public Thread
{
    public:
    LED_matrix(pthread_t ID);
    virtual ~LED_matrix();
    int8_t init(void);
    void write_val(uint8_t *row_val);
    void refresh(void);
    private:
    MCP23S17 *GPIO;
    timer_t timerid;
    static void TIMER_handler(int sig, siginfo_t *si, void *uc);
    void run(void);
    uint8_t v_buffer[LED_matrix_Row];
    uint8_t row;
};


#endif
