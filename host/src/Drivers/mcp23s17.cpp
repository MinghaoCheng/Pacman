#include "mcp23s17.h"
#include "mcp23x17_reg.h"

#define SPI_MODE  0
#define SPI_SPEED 100
#define SPI_BPW   8

MCP23S17::MCP23S17(uint8_t channel, pthread_t id):Thread(id)
{
    this->spi_dev = new SPI(channel, SPI_SPEED, SPI_MODE, SPI_BPW);
}

MCP23S17::~MCP23S17()
{
    delete this->spi_dev;
}

int8_t MCP23S17::init(IRQ_callback int_handler)
{
    if(-1 < spi_dev->Open())
    {
        this->write_reg(0, IOCON, IOCON_SEQOP | IOCON_INTPOL); // disable sequential mode, int active high
        this->write_reg(0, IOCONB,IOCON_SEQOP | IOCON_INTPOL);
        this->write_reg(0, IODIRA, 0xfe);	// set GPIOA.0 as output
        
        
        this->write_reg(0, DEFVALB, 0x00);	// int will be triggered when gpio is high
        this->write_reg(0, INTCONB, 0x01);	// int will be triggered when gpio is against DEFVALB
        this->write_reg(0, GPINTENB, 0x01); // enable int
        
        this->INT_handler = int_handler;
        
        printf("SPI init OK\n");
        return 0;
    }
    else
    {
        printf("SPI init failed\n");
        return -1;
    }
}

void MCP23S17::run(void)
{
    int GPIO_fd;
    char status;
    
    struct pollfd fds[1];
    int poll_ret;
    

    GPIO_fd = open("/dev/GPIO_INT", O_RDWR,S_IRUSR | S_IWUSR);
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
            status = read(fds[0].fd, &status, 1);
            if(status == 1)
            {
                this->INT_handler(status);
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

void MCP23S17::test_bit(uint8_t value)
{
    if(value)
    {
		this->write_reg(0, GPIOA, 0x01);
        this->write_reg(0, OLATA, 0x01);
    }
    else
    {
		this->write_reg(0, GPIOA, 0x00);
        this->write_reg(0, OLATA, 0x00);
    }
}
