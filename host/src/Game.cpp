/*
 * GNU GENERAL PUBLIC LICENSE
 * Version 1, April 2019
 *
 * (C) 2019, Minghao Cheng <chengminghao92@gmail.com>
 */
#include "Game.h"

Game::Game(pthread_t ID):Thread(ID),timer()
{
    this->id = ID;
    this->led_matrix = new LED_matrix(ID + 1);
    this->ldr_matrix = new LDR_matrix(ID + 2, (callback *)this);
    this->game_running = false;
}

Game::~Game(void)
{
    delete this->led_matrix;
}

int8_t Game::init(void)
{
    printf("Game: Initialising\n");
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
    if(-1 == this->sound_dev->init())
    {
        return -1;
    }
    printf("Game: LDR Sound_dev ok\n");
    
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
    // set the flag
    this->game_running = true;
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
    }
}

void Game::timerEvent(void)
{
    if(this->game_running)
    {
        this->ghost_position_update();
    }
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
        // all dots are eaten, game complete
        // play winning music
        // this->sound_dev->play_win();
        this->game_running = false;
    }
    // check whether the car is hit by the monster
    if(this->car_position.column == this->ghost_position.column && this->car_position.row == this->ghost_position.row)
    {
        val = -1;
        // pacman is catched by monster, game over
        // play losing music
        // this->sound_dev->play_lost();
        printf("Pacman has been catched by the ghost\n");
        this->game_running = false;
    }
    return val;
}

void Game::ghost_position_update(void)
{
    static bool move_column = true;
    if(move_column && (this->ghost_position.column != this->car_position.column))
    {
        if(this->ghost_position.column > this->car_position.column)
        {
            this->ghost_position.column -= 1;
        }
        else if(this->ghost_position.column < this->car_position.column)
        {
            this->ghost_position.column += 1;
        }
    }
    else
    {
        if(this->ghost_position.row > this->car_position.row)
        {
            this->ghost_position.row -= 1;
        }
        else if(this->ghost_position.row < this->car_position.row)
        {
            this->ghost_position.row += 1;
        }
    }
    if(this->ghost_position.row >= GAME_PANEL_MATRIX_ROW)
    {
        this->ghost_position.row = GAME_PANEL_MATRIX_ROW - 1;
    }
    if(this->ghost_position.column >= GAME_PANEL_MATRIX_COLUMN)
    {
        this->ghost_position.column = GAME_PANEL_MATRIX_COLUMN - 1;
    }
    if(this->ghost_position.row < 0)
    {
        this->ghost_position.row = 0;
    }
    if(this->ghost_position.column < 0)
    {
        this->ghost_position.column = 0;
    }
    move_column = !move_column;
    v_buffer[this->ghost_position.row][1] = 1<<this->ghost_position.column;
    printf("Game:Ghost position, row = %d, column = %d. Car position, row = %d, column = %d\n", this->ghost_position.row, this->ghost_position.column, this->car_position.row, this->car_position.column);
    game_status();
}
