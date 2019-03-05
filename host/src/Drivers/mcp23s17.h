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

typedef void (*IRQ_callback) (uint8_t GPIO_value);

class MCP23S17:public Thread
{
	public:
	MCP23S17(uint8_t channel, pthread_t id);
	~MCP23S17();
	
	int8_t init(IRQ_callback cb);
	void test_bit(uint8_t value);
		
	private:
	SPI *spi_dev;
	IRQ_callback INT_handler;
	void run(void);
	void write_reg(uint8_t address, uint8_t reg, uint8_t value);
    uint8_t read_reg(uint8_t address, uint8_t reg);
};

#endif
