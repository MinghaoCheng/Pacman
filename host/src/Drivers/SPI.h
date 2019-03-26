#ifndef _SPI_H_
#define _SPI_H_

#include <stdint.h>
#include <linux/spi/spidev.h>

/*
 * Description:
 * SPI bus driver in user space using spidev.h
 */
class SPI
{
    public:
    SPI(uint8_t channel, uint32_t speed, uint8_t mode, uint8_t bits_per_word);
    ~SPI();
    int8_t Open(void);
    int8_t ReadWrite_Buffer(uint8_t *tx_buffer, uint8_t *rx_buffer, uint8_t length);
    
    private:
    int16_t FD;
    uint8_t Channel;
    uint8_t Mode;
    
    struct spi_ioc_transfer spi_msg;
    static constexpr const char* spi_dev_0 = "/dev/spidev0.0";
    static constexpr const char* spi_dev_1 = "/dev/spidev0.1";
};

#endif
