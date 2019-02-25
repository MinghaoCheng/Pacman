#include "mcp23s17.h"
#include "mcp23x0817.h"

#include "SPI.h"
#include "stdio.h"

SPI *spi_dev;

void mcp_write_byte(uint8_t devId, uint8_t reg, uint8_t byte)
{
	uint8_t spiData [3] ;

	spiData [0] = CMD_WRITE | ((devId & 7) << 1) ;
	spiData [1] = reg ;
	spiData [2] = byte ;
	
	spi_dev->ReadWrite_Buffer(spiData, spiData, 3);
}

uint8_t mcp_read_byte(uint8_t devId, uint8_t reg)
{
	uint8_t spiData [3] ;

	spiData [0] = CMD_READ | ((devId & 7) << 1) ;
	spiData [1] = reg ;

	spi_dev->ReadWrite_Buffer(spiData, spiData, 3) ;

	return spiData [2] ;
}

void mcp_init(void)
{
	spi_dev = new SPI(0, 1000, 0, 8);
	if(-1 < spi_dev->Open())
	{
		mcp_write_byte(0, MCP23x17_IOCON, IOCON_SEQOP);
                mcp_write_byte(0, MCP23x17_IOCONB,IOCON_SEQOP);
		mcp_write_byte(0, MCP23x17_IODIRA, 0xfe);
		printf("SPI init OK\n");
	}
	else
	{
		printf("SPI init failed\n");
	}
}

void test_high(void)
{
        mcp_write_byte(0, MCP23x17_GPIOA, 0x01);
	mcp_write_byte(0, MCP23x17_OLATA, 0x01);
}

void test_low(void)
{
        mcp_write_byte(0, MCP23x17_GPIOA, 0x00);
	mcp_write_byte(0, MCP23x17_OLATA, 0x00);
}
uint8_t read_test(uint8_t addr)
{
	return mcp_read_byte(0, addr);
}
