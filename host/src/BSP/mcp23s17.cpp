/*
 * GNU GENERAL PUBLIC LICENSE
 * Version 1, Feb 2019
 *
 * (C) 2019, Minghao Cheng <chengminghao92@gmail.com>
 */
#include "mcp23s17.h"
#include "mcp23x17_reg.h"

/*
 * Description
 * SPI params and addr specification
 */
#define SPI_MODE            0
#define SPI_SPEED           4000000
#define SPI_BPW             8

#define DEV_ADDR            0x0

bool MCP23S17::Is_reset = false;

/**
 * Constructor for the output device
 * 
 * @param  SPI_channel
 * @return None
 */
MCP23S17::MCP23S17(uint8_t SPI_channel)
{
    this->spi_channel = SPI_channel;
    this->spi_dev = new SPI(SPI_channel, SPI_SPEED, SPI_MODE, SPI_BPW);
    this->dev_mode = OUTPUT_DEV;
    this->addr = DEV_ADDR;
}

/**
 * Constructor for input device>
 * 
 * @param  SPI_channel
 * @param  pointer of the callback function
 * @param  pthread ID
 * @return None
 */
MCP23S17::MCP23S17(uint8_t SPI_channel, callback *INT_callback_func, pthread_t id):Thread(id)
{
    this->INT_callback = INT_callback_func;
    this->spi_channel = SPI_channel;
    this->spi_dev = new SPI(SPI_channel, SPI_SPEED, SPI_MODE, SPI_BPW);
    this->dev_mode = INPUT_DEV;
    this->addr = DEV_ADDR;
}

/**
 * Destructor, delete instance of SPI driver
 *
 * @param  None
 * @return None
 */
MCP23S17::~MCP23S17()
{
    delete this->spi_dev;
}

/**
 * Initialisation function, reset device --> set device control register --> set params
 * 
 * @param  None
 * @return result of the init, -1 --> failed, 0 --> success
 */
int8_t MCP23S17::init(void)
{
    // reset mcp23s17
    this->Reset();
    // Open SPI
    if(-1 < spi_dev->Open())
    {
        this->write_reg(this->addr, IOCON, IOCON_SEQOP | IOCON_INTPOL);  // disable sequential mode, int active high
        this->write_reg(this->addr, IOCONB,IOCON_SEQOP | IOCON_INTPOL);  // disable sequential mode, int active high
    }
    else
    {
        printf("MCP23S17: SPI init failed\n");
        return -1;
    }
    // test connection
    if((IOCON_SEQOP | IOCON_INTPOL) != this->read_reg(this->addr, IOCON))
    {
        printf("MCP23S17: GPIO extender testing failed, channel = %d\n", this->spi_channel);
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
        // set GPIOA and GPIOB as input
        this->write_reg(this->addr, IODIRA, 0xff);
        this->write_reg(this->addr, IODIRB, 0xff);
        // Enable pull-up resistors
        this->write_reg(this->addr, GPPUA, 0xff);
        this->write_reg(this->addr, GPPUB, 0xff);
        // Open interrupt-on-change function
        this->write_reg(this->addr, GPINTENA, 0xff);
        this->write_reg(this->addr, GPINTENB, 0xff);
        // Compare pin values with DEFVAL register
        //this->write_reg(this->addr, INTCONA, 0xff);
        //this->write_reg(this->addr, INTCONB, 0xff);
        // Set DEFVAL register
        //this->write_reg(this->addr, DEFVALA, 0xff);
        //this->write_reg(this->addr, DEFVALB, 0xff);
    }
    // Open the device file
    this->fds.fd = open("/dev/Pacman_dev", O_RDWR,S_IRUSR | S_IWUSR);
    if(this->fds.fd == -1)
    {
        printf("MCP23S17: Pac_dev file open failed\n");
        return -1;
    }
    
    return 0;
}

/**
 * Set the output value of GPIOA
 * 
 * @param  value of GPIOA
 * @return None
 */
void MCP23S17::Set_GPIOA(uint8_t val)
{
    this->write_reg(this->addr, GPIOA, val);
    this->write_reg(this->addr, OLATA, val);
}

/**
 * Set the output value of GPIOB
 * 
 * @param  value of GPIOB
 * @return None
 */
void MCP23S17::Set_GPIOB(uint8_t val)
{
    this->write_reg(this->addr, GPIOB, val);
    this->write_reg(this->addr, OLATB, val);
}

/**
 * Reset device, all devices would be reset together, this function will only reset all devices once
 * 
 * @param  None
 * @return None
 */
void MCP23S17::Reset(void)
{
    char val;
    int GPIO_fd;
    if(!MCP23S17::Is_reset)
    {
        MCP23S17::Is_reset = true;
        GPIO_fd = open("/dev/Pacman_dev", O_RDWR,S_IRUSR | S_IWUSR);
        if(GPIO_fd == -1)
        {
            printf("MCP23S17: Pac_dev file open failed\n");
        }
        val = 0x00;
        write(GPIO_fd, &val, 1);
        usleep(10000);            //hold for 10 ms
        val = 0x01;
        write(GPIO_fd, &val, 1);
    }
}

/**
 * Thread function, poll the Pac_dev device file and handle INT
 *
 * @param  None
 * @return None
 */
void MCP23S17::thread_handler(void)
{
    char status, val;
    int poll_ret;
    
    this->fds.events = POLLPRI;
    while(1)
    {
        poll_ret = poll(&this->fds, 1, -1);
        if(poll_ret != 0)
        {
            status = read(this->fds.fd, &val, 1);
            if(status)
            {
                this->INT_handler(val);
            }
        }
    }
}

/**
 * Handler of the INT
 * 
 * @param  None
 * @return None
 */
void MCP23S17::INT_handler(uint8_t val)
{
    uint8_t buffer[2];
    buffer[0] = ~this->read_reg(this->addr, GPIOA);
    buffer[1] = ~this->read_reg(this->addr, GPIOB);
    this->INT_callback->cb_func(buffer, 2);
}

/**
 * Low level function, write MCP23S17 register through SPI
 * 
 * @param  address of the device
 * @param  address of the register
 * @param  value of the register
 * @return None
 */
void MCP23S17::write_reg(uint8_t address, uint8_t reg, uint8_t value)
{
    uint8_t spiData [3] ;

    spiData [0] = CMD_WRITE | ((address & 7) << 1);
    spiData [1] = reg;
    spiData [2] = value;
    
    this->spi_dev->ReadWrite_Buffer(spiData, spiData, 3);
}

/**
 * Low level function, read MCP23S17 register through SPI
 * 
 * @param  address of the device
 * @param  address of the register
 * @return value of the register
 */
uint8_t MCP23S17::read_reg(uint8_t address, uint8_t reg)
{
    uint8_t spiData [3] ;

    spiData [0] = CMD_READ | ((address & 7) << 1) ;
    spiData [1] = reg ;

    this->spi_dev->ReadWrite_Buffer(spiData, spiData, 3) ;

    return spiData [2] ;
}

