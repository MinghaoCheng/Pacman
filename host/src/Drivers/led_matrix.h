#ifndef _LED_MATRIX_H_
#define _LED_MATRIX_H_

#include <stdint.h>
#include <signal.h> 
#include <unistd.h> 

#include "brd_config.h"
#include "mcp23s17.h"
#include "../util/Thread.h"

class LED_matrix:public Thread
{
    public:
    LED_matrix(pthread_t ID);
    virtual ~LED_matrix();
    int8_t init(void);
    void write_val(uint8_t *row);
    void refresh(void);
    private:
    MCP23S17 *GPIO;
    static void TIMER_handler(int sig, siginfo_t *si, void *uc);
    void run(void);
    uint8_t v_buffer[LED_matrix_Row];
    uint8_t column;
};


#endif
