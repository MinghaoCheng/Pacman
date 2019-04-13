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
LDR_matrix::LDR_matrix(pthread_t ID, callback *INT_callback)
{
    this->id = ID;
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
    this->GPIO->thread_start();
    return 0;
}

/*
 * Description:
 * GPIO INT callback
 */
void LDR_matrix::cb_func(uint8_t *param, uint8_t size)
{
    //printf("LDR: INT call_back, GPIOA = %x, GPIOB = %x\n", param[0], param[1]);
    uint8_t buf[LED_MATRIX_ROW];
    // mapping GPIO to rows
    buf[0] = param[0]&0x0f;
    buf[1] = (param[0]&0xf0)>>4;
    buf[2] = param[1]&0x0f;
    buf[3] = (param[1]&0xf0)>>4;
    // call the INT_handler
    this->INT_handler->cb_func(buf, LED_MATRIX_ROW);
}
