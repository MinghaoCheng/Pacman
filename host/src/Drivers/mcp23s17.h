#ifndef _MCP23S17_H_
#define _MCP23S17_H_

#include <stdio.h>
#include <stdint.h>
#include <pthread.h>

#include <fcntl.h>
#include <unistd.h>
#include <poll.h>

#include "SPI.h"
#include "../util/Thread.h"
typedef enum 
{
    INPUT_DEV = 0,
    OUTPUT_DEV = 1
}Dev_mode;

class MCP23S17:public Thread
{
    public:
    MCP23S17(uint8_t channel);
    MCP23S17(uint8_t channel, pthread_t id);
    virtual ~MCP23S17();
	
    int8_t init(void);
    void Set_GPIOA(uint8_t val);
    void Set_GPIOB(uint8_t val);
	
    private:
    Dev_mode dev_mode;
    SPI *spi_dev;
    void run(void);
    void write_reg(uint8_t address, uint8_t reg, uint8_t value);
    uint8_t read_reg(uint8_t address, uint8_t reg);
};

#endif
