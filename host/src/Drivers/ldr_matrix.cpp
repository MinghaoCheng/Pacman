/*
 * GNU GENERAL PUBLIC LICENSE
 * Version 1, Mar 2019
 *
 * (C) 2019, Minghao Cheng <chengminghao92@gmail.com>
 */
#include "ldr_matrix.h"

/*
 * Description:
 * Contructor
 * Param:
 * Pointer of the callback instance
 */
LDR_matrix::LDR_matrix(callback *INT_callback):Thread(ID)
{
    this->INT_handler = INT_callback;
    this->GPIO = new MCP23S17(LDR_GPIO_SPI_CHANNEL, (callback *)this, this->id);
}

/*
 * Description:
 * Destructor
 */
LDR_matrix::~LDR_matrix(void)
{
    delete this->GPIO;
}

/*
 * Description:
 * Initialise the hardware
 */
int8_t LDR_matrix::init(void)
{
    if(-1 == this->GPIO->init())
    {
        return -1;
    }
    this->GPIO->start();
    return 0;
}

/*
 * Description:
 * GPIO INT callback
 */
void LDR_matrix::cb_func(uint8_t *param, uint8_t size)
{
    printf("LDR: INT call_back, GPIOA = %x, GPIOB = %x\n", param[0], param[1]);
    uint8_t temp,buf[2];
    // row
    if(param[0]&0x0f)
    {
        buf[0] = 0;
    }
    else if(param[0]&0xf0)
    {
        buf[0] = 1;
    }
    if(param[1]&0x0f)
    {
        buf[0] = 2;
    }
    else if(param[1]&0xf0)
    {
        buf[0] = 3;
    }
    // column
    temp = param[0]|param[1];
    for(uint8_t i=0; i<LDR_GPIO_WIDTH; i++)
    {
        if(temp == 1<<i)
        {
            buf[1] = i % 4;
        }
    }
    this->INT_handler->cb_func(buf, 2);
}
