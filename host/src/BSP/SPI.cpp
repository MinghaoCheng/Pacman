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

/**
 * class constructor, set params
 * 
 * @param  SPI channel
 * @param  SPI speed
 * @param  SPI mode
 * @param  SPI bits per word
 * @return None
 */
SPI::SPI(uint8_t channel, uint32_t speed, uint8_t mode, uint8_t bits_per_word)
{
    this->Channel = channel;
    this->Mode = mode;
    this->Speed = speed;
    this->BPW = bits_per_word;
    
    memset(&this->spi_msg, 0, sizeof (this->spi_msg));
    this->spi_msg.bits_per_word = this->BPW;
    this->spi_msg.speed_hz = this->Speed;
    this->spi_msg.delay_usecs = 0;
}

/**
 * class destructor, close device file
 * 
 * @param  None
 * @return None
 */
SPI::~SPI()
{
    close(this->FD);
}

/**
 * open SPI and set params
 * 
 * @param  None
 * @return fd of the device file
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

/**
 * write and read buffer simultaneously
 * 
 * @param  pointer to the tx_buffer
 * @param  pointer to the rx_buffer
 * @param  length of the data
 * @return result of the communication
 */
int8_t SPI::ReadWrite_Buffer(uint8_t *tx_buffer, uint8_t *rx_buffer, uint8_t length)
{
    this->spi_msg.tx_buf = (unsigned long)tx_buffer;
    this->spi_msg.rx_buf = (unsigned long)rx_buffer;
    this->spi_msg.len = length;
    
    return ioctl (this->FD, SPI_IOC_MESSAGE(1), &this->spi_msg);
}
