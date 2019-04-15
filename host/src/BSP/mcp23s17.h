/*
 * GNU GENERAL PUBLIC LICENSE
 * Version 1, Feb 2019
 *
 * (C) 2019, Minghao Cheng <chengminghao92@gmail.com>
 */
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
#include "../util/callback.h"

/**
 * enum to indicate the type of the device>
 */
typedef enum 
{
    INPUT_DEV = 0,
    OUTPUT_DEV = 1
}Dev_mode;

/**
 * Class of MCP23S17 driver
 */
class MCP23S17:public Thread
{
    public:
    MCP23S17(uint8_t SPI_channel);
    MCP23S17(uint8_t SPI_channel, callback *INT_callback_func, pthread_t id);
    virtual ~MCP23S17();
	
    int8_t init(void);
    void Set_GPIOA(uint8_t val);
    void Set_GPIOB(uint8_t val);

    static bool Is_reset;

    private:
    Dev_mode dev_mode;
    SPI *spi_dev;
    uint8_t spi_channel;
    uint8_t addr;
    void Reset(void);
    void thread_handler(void);
    void INT_handler(uint8_t val);
    void write_reg(uint8_t address, uint8_t reg, uint8_t value);
    uint8_t read_reg(uint8_t address, uint8_t reg);
    callback *INT_callback;
    
    struct pollfd fds;
};

#endif
