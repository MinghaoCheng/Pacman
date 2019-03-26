#include "ldr_matrix.h"

LDR_matrix::LDR_matrix(pthread_t ID, callback *INT_callback):Thread(ID)
{
    this->id = ID;
    this->INT_handler = INT_callback;
    this->GPIO = new MCP23S17(LDR_GPIO_SPI_CHANNEL, (callback *)this, this->id);
}

LDR_matrix::~LDR_matrix(void)
{
    delete this->GPIO;
}

int8_t LDR_matrix::init(void)
{
    if(-1 == this->GPIO->init())
    {
        return -1;
    }
    this->GPIO->start();
    return 0;
}

void LDR_matrix::run(void)
{
    
}

void LDR_matrix::cb_func(uint8_t *param, uint8_t size)
{
    printf("LDR call_back!\n");
    this->INT_handler->cb_func(param, size);
}
