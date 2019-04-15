/*
 * GNU GENERAL PUBLIC LICENSE
 * Version 1, Mar 2019
 *
 * (C) 2019, Minghao Cheng <chengminghao92@gmail.com>
 */
#include "ldr_matrix.h"

/**
 * Contructor
 * Poll the device file, call the callback function when interrupt triggered
 * 
 * @param  ID of the thread
 * @param  Pointer of the callback instance
 * @return None
 */
LDR_matrix::LDR_matrix(pthread_t ID, callback *INT_callback)
{
    this->id = ID;
    this->INT_handler = INT_callback;
    this->GPIO = new MCP23S17(LDR_GPIO_SPI_CHANNEL, (callback *)this, this->id);
}

/**
 * Destructor
 * 
 * @param  None
 * @return None
 */
LDR_matrix::~LDR_matrix(void)
{
    delete this->GPIO;
}

/**
 * Initialise the hardware
 * 
 * @param  None
 * @return result, -1 --> failed, 0 --> success
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

/**
 * GPIO INT callback
 * 
 * @param data pass to the callback function
 * @param size of the data
 * @return None
 */
void LDR_matrix::cb_func(uint8_t *param, uint8_t size)
{
    uint8_t columns[LED_MATRIX_ROW];
    uint8_t rows[LED_MATRIX_ROW];
    // mapping GPIO to rows
    columns[0] = 0xf&~(param[0]&0x0f);
    columns[1] = 0xf&~((param[0]&0xf0)>>4);
    columns[2] = 0xf&~(param[1]&0x0f);
    columns[3] = 0xf&~((param[1]&0xf0)>>4);

    rows[0] =  (columns[0]&1)     |((columns[1]&1)<<1)  | ((columns[2]&1)<<2) | ((columns[3]&1)<<3);
    rows[1] = ((columns[0]&2)>>1) | (columns[1]&2)      | ((columns[2]&2)<<1) | ((columns[3]&2)<<2);
    rows[2] = ((columns[0]&4)>>2) | ((columns[1]&4)>>1) |  (columns[2]&4)     | ((columns[3]&4)<<1);
    rows[3] = ((columns[0]&8)>>3) | ((columns[1]&8)>>2) | ((columns[2]&8)>>1) |  (columns[3]&8);
    
    // call the INT_handler
    this->INT_handler->cb_func(rows, LED_MATRIX_ROW);
}
