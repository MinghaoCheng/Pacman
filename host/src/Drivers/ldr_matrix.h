#ifndef _LDR_MATRIX_
#define _LDR_MATRIX_

#include <stdint.h>

#include "./../Drivers/mcp23s17.h"
#include "./../util/Thread.h"
#include "./../util/callback.h"
#include "brd_config.h"

class LDR_matrix: public Thread, public callback
{
    public:
    LDR_matrix(pthread_t ID, callback *INT_callback);
    virtual ~LDR_matrix(void);
    int8_t init(void);
    
    private:
    pthread_t id;
    MCP23S17 *GPIO;
    virtual void run(void);
    virtual void cb_func(uint8_t *param, uint8_t size);
    callback *INT_handler;
};

#endif
