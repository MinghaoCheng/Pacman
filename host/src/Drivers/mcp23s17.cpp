#include "mcp23s17.h"
#include "mcp23x17_reg.h"

#define SPI_MODE    0
#define SPI_SPEED   4000000
#define SPI_BPW     8

#define DEV_ADDR    0

MCP23S17::MCP23S17(uint8_t channel)
{
    this->INT_handler = NULL;
    this->spi_dev = new SPI(channel, SPI_SPEED, SPI_MODE, SPI_BPW);
}

MCP23S17::MCP23S17(uint8_t channel, IRQ_callback int_handler, pthread_t id):Thread(id)
{
    this->INT_handler = int_handler;
    this->spi_dev = new SPI(channel, SPI_SPEED, SPI_MODE, SPI_BPW);
}

MCP23S17::~MCP23S17()
{
    delete this->spi_dev;
}

int8_t MCP23S17::init(void)
{
    // Open SPI
    if(-1 < spi_dev->Open())
    {
        this->write_reg(0, IOCON, IOCON_SEQOP | IOCON_INTPOL);  // disable sequential mode, int active high
        this->write_reg(0, IOCONB,IOCON_SEQOP | IOCON_INTPOL);  // disable sequential mode, int active high
        
        printf("SPI init OK\n");
        return 0;
    }
    else
    {
        printf("SPI init failed\n");
        return -1;
    }
    
    // This is a output device
    if(this->INT_handler == NULL)
    {
        this->write_reg(0, 
    }
    // This is a input device
    else
    {
        
    }
}

void MCP23S17::Set_GPIOA(uint8_t val)
{
    
}

void MCP23S17::Set_GPIOB(uint8_t val)
{
    
}

void MCP23S17::run(void)
{
    int GPIO_fd;
    char status, val;
    
    struct pollfd fds[1];
    int poll_ret;
    

    GPIO_fd = open("/dev/Pacman_dev", O_RDWR,S_IRUSR | S_IWUSR);
    if(GPIO_fd == -1)
    {
        printf("file open failed\n");
    }
    
    fds[0].fd = GPIO_fd;
    fds[0].events = POLLPRI;
    
    while(1)
    {
        poll_ret = poll(fds, 1, -1);
        if(poll_ret != 0)
        {
            status = read(fds[0].fd, &val, 1);
            if(status)
            {
                this->INT_handler(val);
            }
        }
    }
}
    
void MCP23S17::write_reg(uint8_t address, uint8_t reg, uint8_t value)
{
    uint8_t spiData [3] ;

    spiData [0] = CMD_WRITE | ((address & 7) << 1);
    spiData [1] = reg;
    spiData [2] = value;
    
    this->spi_dev->ReadWrite_Buffer(spiData, spiData, 3);
}

uint8_t MCP23S17::read_reg(uint8_t address, uint8_t reg)
{
    uint8_t spiData [3] ;

    spiData [0] = CMD_READ | ((address & 7) << 1) ;
    spiData [1] = reg ;

    this->spi_dev->ReadWrite_Buffer(spiData, spiData, 3) ;

    return spiData [2] ;
}
