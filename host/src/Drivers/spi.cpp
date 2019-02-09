#include <stdio.h>

#include "spi.h"


int SPI::init(void)
{
	printf("SPI initialied\n");
	return 0;
}
int SPI::transfer_bulk(uint8_t *write_buffer, uint8_t nbytes, uint8_t *read_buffer)
{
	printf("SPI transfered\n");
	return 0;
}
