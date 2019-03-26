#include "mcp23s17.h"
#include "mcp23x17_reg.h"

/*
 * Description:
 * SPI params and addr specification
 */
#define SPI_MODE    0
#define SPI_SPEED   4000000
#define SPI_BPW     8

#define INPUT_DEV_ADDR          0x0
#define OUTPUT_DEV_ADDR         0x7

/*
 * Description:
 * Constructor for output device
 */
MCP23S17::MCP23S17(uint8_t SPI_channel)
{
    this->spi_channel = SPI_channel;
    this->spi_dev = new SPI(SPI_channel, SPI_SPEED, SPI_MODE, SPI_BPW);
    this->dev_mode = OUTPUT_DEV;
    this->addr = OUTPUT_DEV_ADDR;
}

/*
 * Description:
 * Constructor for input device
 */
MCP23S17::MCP23S17(uint8_t SPI_channel, callback *INT_callback, pthread_t id):Thread(id)
{
    this->INT_handler = INT_callback;
    this->spi_channel = SPI_channel;
    this->spi_dev = new SPI(SPI_channel, SPI_SPEED, SPI_MODE, SPI_BPW);
    this->dev_mode = INPUT_DEV;
    this->addr = INPUT_DEV_ADDR;
}

/*
 * Description:
 * Destructor, delete instance of SPI driver
 */
MCP23S17::~MCP23S17()
{
    delete this->spi_dev;
}

/*
 * Description:
 * Initialisation function, reset device --> set device control register --> set params
 */
int8_t MCP23S17::init(void)
{
    // reset mcp23s17
    MCP23S17::Reset();
    // Open SPI
    if(-1 < spi_dev->Open())
    {
        this->write_reg(this->addr, IOCON, IOCON_SEQOP | IOCON_INTPOL);  // disable sequential mode, int active high
        this->write_reg(this->addr, IOCONB,IOCON_SEQOP | IOCON_INTPOL);  // disable sequential mode, int active high
    }
    else
    {
        printf("SPI init failed\n");
        return -1;
    }
    // test connection
    if((IOCON_SEQOP | IOCON_INTPOL) != this->read_reg(this->addr, IOCON))
    {
        printf("GPIO extender testing failed, channel = %d\n", this->spi_channel);
        return -1;
    }    
    // This is a output device
    if(this->dev_mode == OUTPUT_DEV)
    {
        // set GPIOA and GPIOB as output
        this->write_reg(this->addr, IODIRA, 0x00);
        this->write_reg(this->addr, IODIRB, 0x00);
    }
    // This is a input device
    else
    {
    }
    
    return 0;
}

/*
 * Description:
 * Set value of GPIOA
 */
void MCP23S17::Set_GPIOA(uint8_t val)
{
    this->write_reg(this->addr, GPIOA, val);
    this->write_reg(this->addr, OLATA, val);
}

/*
 * Description:
 * Set value of GPIOA
 */
void MCP23S17::Set_GPIOB(uint8_t val)
{
    this->write_reg(this->addr, GPIOB, val);
    this->write_reg(this->addr, OLATB, val);
}

/*
 * Description:
 * Reset device, all devices would be reset together, this function will only reset all devices once
 */
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
            printf("Pac_dev file open failed\n");
        }
        val = 0x00;
        write(GPIO_fd, &val, 1);
        usleep(10000);            //hold for 10 ms
        val = 0x01;
        write(GPIO_fd, &val, 1);
    }
}

/*
 * Description:
 * Thread function, poll the Pac_dev device file and handle INT
 */
void MCP23S17::run(void)
{
    char status, val;
    
    struct pollfd fds[1];
    int poll_ret;
    

    fds[0].fd = open("/dev/Pacman_dev", O_RDWR,S_IRUSR | S_IWUSR);
    if(fds[0].fd == -1)
    {
        printf("Pac_dev file open failed\n");
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
                this->INT_handler->cb_func((uint8_t *)&val, 1);
            }
        }
    }
}

/*
 * Description:
 * Low level function, write MCP23S17 register
 */
void MCP23S17::write_reg(uint8_t address, uint8_t reg, uint8_t value)
{
    uint8_t spiData [3] ;

    spiData [0] = CMD_WRITE | ((address & 7) << 1);
    spiData [1] = reg;
    spiData [2] = value;
    
    this->spi_dev->ReadWrite_Buffer(spiData, spiData, 3);
}

/*
 * Description:
 * Low level function, read MCP23S17 register
 */
uint8_t MCP23S17::read_reg(uint8_t address, uint8_t reg)
{
    uint8_t spiData [3] ;

    spiData [0] = CMD_READ | ((address & 7) << 1) ;
    spiData [1] = reg ;

    this->spi_dev->ReadWrite_Buffer(spiData, spiData, 3) ;

    return spiData [2] ;
}

