/*
 * GNU GENERAL PUBLIC LICENSE
 * Version 1, April 2019
 *
 * (C) 2019, Minghao Cheng <chengminghao92@gmail.com>
 */
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

void Game::reset(void)
{
    for (uint8_t i = 0; i < GAME_PANEL_MATRIX_ROW; i++)
    {
        this->v_buffer[i][0] = 0xff;
        this->v_buffer[i][1] = 0x00;
        this->led_matrix->write_green(i, this->v_buffer[i][0]);
        this->led_matrix->write_red(i, this->v_buffer[i][1]);
    }
    // let monster be at location (0,1)
    this->v_buffer[0][1] = 0x01;
    this->led_matrix->write_red(0, this->v_buffer[0][1]);
    // reset the position of the car
    this->car_position.Is_upon_dot = false;
    this->car_position.column = 0;
    this->car_position.row = 0;
    // reset the position of the ghost
    this->ghost_position.column = GAME_PANEL_MATRIX_COLUMN - 1;
    this->ghost_position.row = GAME_PANEL_MATRIX_ROW - 1;
    // play start music
    // this->sound_dev->play_start();
}

void Game::thread_handler(void)
{
    this->reset();
    this->led_matrix->thread_start();
    this->start_timer(GHOST_UPDATE_PERIOD_NS);
    printf("Game: Start\n");
    while(1)
    {
        sleep(100);
    }
}

void Game::cb_func(uint8_t *param, uint8_t size)
{
    int8_t status;
    // called by keyboard
    if(size == 1)
    {
        // send corresponding instruction to the car
        // ........
        // reset game
        // ........
    }
    // called by LDR matrix
    else
    {
        for(uint8_t i=0; i<size; i++)
        {
            this->v_buffer[i][2] = param[i];
        }
        // update position of the car
        for(uint8_t i=0; i<GAME_PANEL_MATRIX_ROW; i++)
        {
            if(this->v_buffer)
            {
                this->car_position.row = i;
                for(uint8_t j=0; j<GAME_PANEL_MATRIX_COLUMN; j++)
                {
                    if(this->v_buffer[this->car_position.row][2] == 1<<j)
                    {
                        this->car_position.column = j;
                        break;
                    }
                }
                break;
            }
        }
        // check game status
        status = game_status();
        switch(status)
        {
            case 0:
            {
                // no event triggered
                break;
            }
            case 1:
            {
                // all dots are eaten, game complete
                // play winning music
                // this->sound_dev->play_win();
                break;
            }
            case -1:
            {
                // pacman is catched by monster, game over
                // play losing music
                // this->sound_dev->play_lost();
                break;
            }
        }
    }
}

void Game::timerEvent(void)
{
    this->monster_update();
}

int8_t Game::game_status(void)
{
    uint8_t val = 0;
    uint8_t temp = 0;
    // check whether all dots are eaten
    for (uint8_t i=0; i<LED_MATRIX_ROW; i++)
    {
        temp += v_buffer[i][0];
    }
    if(temp == 0)
    {
        val = 1;
    }
    // check whether the car is hit by the monster
    if(this->car_position.column == this->ghost_position.column && this->car_position.row == this->ghost_position.row)
    {
        val = -1;
    }
    return val;
}

void Game::monster_update(void)
{
    static bool move_column = true;
    if(move_column && this->ghost_position.column != this->car_position.column)
    {
        if(this->ghost_position.column > this->car_position.column)
        {
            this->ghost_position.column += 1;
        }
        else if(this->ghost_position.column < this->car_position.column)
        {
            this->ghost_position.column -= 1;
        }
    }
    else
    {
        if(this->ghost_position.row > this->car_position.row)
        {
            this->ghost_position.column += 1;
        }
        else if(this->ghost_position.row < this->car_position.row)
        {
            this->ghost_position.column -= 1;
        }
    }
    move_column = ~move_column;
    
}
