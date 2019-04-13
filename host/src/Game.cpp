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
    this->tcp_dev = new TCP_dev(ID + 3);
    this->key = new Keyboard(ID + 4, (callback *)this);
    this->sound_dev = new Sound_dev(ID + 5);
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
    if(-1 == this->tcp_dev->init())
    {
        return -1;
    }
    printf("Game: TCP/IP ok\n");
    if(-1 == this->key->init())
    {
        return -1;
    }
    printf("Game: Keyboard ok\n");
    
    return 0;
}

void Game::reset(void)
{
    // stop the game
    this->game_running = false;
    // reset the pac dots
    for(uint8_t i = 0; i < GAME_PANEL_MATRIX_ROW; i++)
    {
        this->G_LED_buffer[i] = 0xf;
    }
    // reset the position of the car
    this->car_position.Is_upon_dot = false;
    this->car_position.column = 0;
    this->car_position.row = 0;
    // reset the position of the ghost
    this->ghost_position.column = GAME_PANEL_MATRIX_COLUMN - 1;
    this->ghost_position.row = GAME_PANEL_MATRIX_ROW - 1;
    // play start music
    this->sound_dev->play_wav(GAME_START_SOUND_FILE);
    this->update_LED();
    printf("Game: Started\n");
    // set the flag
    this->game_running = true;
}

void Game::thread_handler(void)
{
    this->led_matrix->thread_start();
    this->tcp_dev->thread_start();
    this->key->thread_start();
    this->sound_dev->thread_start();
    this->start_timer(GHOST_UPDATE_PERIOD_NS);
    printf("Game: Press reset to start\n");
    while(1)
    {
        sleep(100);
    }
}

void Game::cb_func(uint8_t *param, uint8_t size)
{
    uint8_t key_val;
    // called by keyboard
    if(size == 1)
    {
        key_val = *param;
        this->tcp_dev->send_buffer((char *)&key_val, 1);
        if(key_val == KEY_RESET)
        {
            this->reset();
        }
    }
    // called by LDR matrix
    else
    {
        if(this->game_running)
        {
            for(uint8_t i=0; i<size; i++)
            {
                this->LDR_buffer[i] = param[i];
            }
            // update position of the car
            this->car_position.Is_upon_dot = false;
            for(uint8_t i=0; i<GAME_PANEL_MATRIX_ROW; i++)
            {
                if(this->LDR_buffer[i])
                {
                    this->car_position.row = i;
                    for(uint8_t j=0; j<GAME_PANEL_MATRIX_COLUMN; j++)
                    {
                        if(this->LDR_buffer[this->car_position.row] == 1<<j)
                        {
                            this->car_position.column = j;
                            this->car_position.Is_upon_dot = true;
                            break;
                        }
                    }
                    break;
                }
            }
            if(this->car_position.Is_upon_dot)
            {
                if(G_LED_buffer[this->car_position.row]&(1<<this->car_position.column))
                {
                    G_LED_buffer[this->car_position.row] &= ~(1<<this->car_position.column);
                    this->sound_dev->play_wav(GAME_DOT_EATEN_FILE);
                }
            }
            this->game_status();
            this->update_LED();
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
    uint8_t temp = 0;
    if((this->car_position.column == this->ghost_position.column) &&
       (this->car_position.row == this->ghost_position.row) &&
       (this->car_position.Is_upon_dot))
    {
        // pacman is catched by monster, game over
        // play losing music
        this->sound_dev->play_wav(GAME_LOSING_FILE);
        printf("GAME: Pacman has been catched by the ghost!\n");
        this->game_running = false;
        return -1;
    }
    // check whether all dots are eaten
    for (uint8_t i=0; i<LED_MATRIX_ROW; i++)
    {
        temp += G_LED_buffer[i];
    }
    if(temp == 0)
    {
        // all dots are eaten, game complete
        // play winning music
        this->sound_dev->play_wav(GAME_WINNING_FILE);
        printf("GAME: All dots have been eaten, you win!!\n");
        this->game_running = false;
        return 1;
    }
    // check whether the car is hit by the monster
    return 0;
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
    this->update_LED();
    printf("Game: Ghost position, row = %d, column = %d. Car position, row = %d, column = %d\n", this->ghost_position.row, this->ghost_position.column, this->car_position.row, this->car_position.column);
    this->game_status();
}

void Game::update_LED(void)
{
    for(uint8_t i=0; i<GAME_PANEL_MATRIX_ROW; i++)
    {
        if(i == this->ghost_position.row)
            this->led_matrix->write_red(this->ghost_position.row, 1<<this->ghost_position.column);
        else
            this->led_matrix->write_red(i, 0);
    }
    for(uint8_t i=0; i<GAME_PANEL_MATRIX_ROW; i++)
    {
        this->led_matrix->write_green(i, this->G_LED_buffer[i]);
    }
}
