#include "mcp23s17.h"
#include "mcp23x17_reg.h"

#define SPI_MODE    0
#define SPI_SPEED   4000000
#define SPI_BPW     8

#define DEV_ADDR    0x07

MCP23S17::MCP23S17(uint8_t channel)
{
    this->spi_dev = new SPI(channel, SPI_SPEED, SPI_MODE, SPI_BPW);
    this->dev_mode = OUTPUT_DEV;
}

MCP23S17::MCP23S17(uint8_t channel, pthread_t id):Thread(id)
{
    this->spi_dev = new SPI(channel, SPI_SPEED, SPI_MODE, SPI_BPW);
    this->dev_mode = INPUT_DEV;
}

MCP23S17::~MCP23S17()
{
    delete this->spi_dev;
}

int8_t MCP23S17::init(void)
{    
    // reset mcp23s17
    MCP23S17::Reset();
    // Open SPI
    if(-1 < spi_dev->Open())
    {
        this->write_reg(DEV_ADDR, IOCON, IOCON_SEQOP | IOCON_INTPOL);  // disable sequential mode, int active high
        this->write_reg(DEV_ADDR, IOCONB,IOCON_SEQOP | IOCON_INTPOL);  // disable sequential mode, int active high
        
        printf("SPI init OK\n");
    }
    else
    {
        printf("SPI init failed\n");
        return -1;
    }
    // This is a output device
    if(this->dev_mode == OUTPUT_DEV)
    {
        // set GPIOA and GPIOB as output
        this->write_reg(DEV_ADDR, IODIRA, 0x00);
        this->write_reg(DEV_ADDR, IODIRB, 0x00);
    }
    // This is a input device
    else
    {
    }
    return 0;
}

void MCP23S17::Set_GPIOA(uint8_t val)
{
    this->write_reg(DEV_ADDR, GPIOA, val);
    this->write_reg(DEV_ADDR, OLATA, val);
}

void MCP23S17::Set_GPIOB(uint8_t val)
{
    this->write_reg(DEV_ADDR, GPIOB, val);
    this->write_reg(DEV_ADDR, OLATB, val);
}

void MCP23S17::Reset(void)
{
    static bool flag = false;
    static int GPIO_fd;
    char val;
    if(!flag)
    {
        flag = true;
        GPIO_fd = open("/dev/Pacman_dev", O_RDWR,S_IRUSR | S_IWUSR);
        if(GPIO_fd == -1)
        {
            printf("GPIO file open failed\n");
        }
        val = 0x00;
        write(GPIO_fd, &val, 1);
        usleep(10000);            //hold for 10 ms
        val = 0x01;
        write(GPIO_fd, &val, 1);
    }
}

void MCP23S17::run(void)
{
    char status, val;
    
    struct pollfd fds[1];
    int poll_ret;
    

    fds[0].fd = open("/dev/Pacman_dev", O_RDWR,S_IRUSR | S_IWUSR);
    if(fds[0].fd == -1)
    {
        printf("file open failed\n");
    }
    
    fds[0].events = POLLPRI;
    
    while(1)
    {
        poll_ret = poll(fds, 1, -1);
        if(poll_ret != 0)
        {
            status = read(fds[0].fd, &val, 1);
            if(status)
            {
                //this->INT_handler(val);
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
