#ifndef _SPI_H_
#define _SPI_H_

#include <stdint.h>

namespace SPI
{
	int init(void);
	int transfer_bulk(uint8_t *write_buffer, uint8_t nbytes, uint8_t *read_buffer);
}

#endif
