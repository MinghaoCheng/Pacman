/*
 * GNU GENERAL PUBLIC LICENSE
 * Version 1, Feb 2019
 *
 * (C) 2019, Minghao Cheng <chengminghao92@gmail.com>
 */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <asm/ioctl.h>

#include "SPI.h"

/*
 * Description:
 * class instructor, set params
 */
SPI::SPI(uint8_t channel, uint32_t speed, uint8_t mode, uint8_t bits_per_word)
{
    this->Channel = channel;
    this->Mode = mode;
    
    this->spi_msg.bits_per_word = bits_per_word;
    this->spi_msg.speed_hz = speed;
    this->spi_msg.delay_usecs = 0;
}

/*
 * Description:
 * class destructor, close device file
 */
SPI::~SPI()
{
    close(this->FD);
}

/*
 * Description:
 * open SPI and set params
 */
int8_t SPI::Open(void)
{
    int8_t fd = 0;
    // open spidev file
    if(this->Channel == 0)
    {
        fd = open(spi_dev_0, O_RDWR);
    }
    else if(this->Channel == 1)
    {
        fd = open(spi_dev_1, O_RDWR);
    }

    if(fd < 0)
    {
        printf("SPI: cannot open spidev, channel = %d\n", this->Channel);
        return fd;
    }

    // set spi parameters
    if (ioctl (fd, SPI_IOC_WR_MODE, &this->Mode) < 0)
    {
        printf("SPI: cannot change mode, channel = %d\n", this->Channel);
        return fd;
    }
    if (ioctl (fd, SPI_IOC_WR_BITS_PER_WORD, &this->spi_msg.bits_per_word) < 0)
    {
        printf("SPI: cannot change BPW, channel = %d\n", this->Channel);
        return fd;
    }
    if (ioctl (fd, SPI_IOC_WR_MAX_SPEED_HZ, &this->spi_msg.speed_hz) < 0)
    {
        printf("SPI: cannot change speed, channel = %d\n", this->Channel);
        return fd;
    }
    
    this->FD = fd;
    return fd;
}

/*
 * Description:
 * write and read buffer simultaneously
 */
int8_t SPI::ReadWrite_Buffer(uint8_t *tx_buffer, uint8_t *rx_buffer, uint8_t length)
{
    this->spi_msg.tx_buf = (unsigned long)tx_buffer;
    this->spi_msg.rx_buf = (unsigned long)rx_buffer;
    this->spi_msg.len = length;
    
    return ioctl (this->FD, SPI_IOC_MESSAGE(1), &this->spi_msg);
}
