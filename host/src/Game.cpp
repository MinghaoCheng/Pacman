#include "Game.h"

Game::Game(pthread_t ID):Thread(ID)
{
    this->id = ID;
    this->led_matrix = new LED_matrix(ID + 1);
    this->ldr_matrix = new LDR_matrix(ID + 2, (callback *)this);
}

Game::~Game(void)
{
    delete this->led_matrix;
}

uint8_t Game::init(void)
{
    printf("Game: initialising\n");
    if(-1 == this->led_matrix->init())
    {
        return -1;
    }
    printf("Game: LED matrix ok\n");
    if(-1 == this->ldr_matrix->init())
    {
        return -1;
    }
    printf("Game: LDR matrix ok\n");
    
    return 0;
}

void Game::led_matrix_refresh(void)
{
    this->led_matrix->refresh();
}

void Game::reset(void)
{
    for (uint8_t i = 0; i < LED_MATRIX_ROW; i++)
    {
        this->vram[i] = 0xff;
    }
    this->led_matrix->write_val(this->vram);
}

void Game::run(void)
{
    //this->led_matrix->start();
    printf("Game: Started\n");
    while(1)
    {
        sleep(1);
    }
}

void Game::cb_func(uint8_t *param, uint8_t size)
{
    uint8_t row = param[0];
    uint8_t column = param[1];
    
    this->vram[row] &= ~(1<<column);
}

