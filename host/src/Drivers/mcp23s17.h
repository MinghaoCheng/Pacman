#ifndef _MCP23S17_H_
#define _MCP23S17_H_

#include <stdint.h>

void mcp_write_byte(uint8_t devId, uint8_t reg, uint8_t byte);
uint8_t mcp_read_byte(uint8_t devId, uint8_t reg);

void mcp_init(void);

void test_high(void);
void test_low(void);
uint8_t read_test(uint8_t addr);

#endif
