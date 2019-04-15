/*
 * GNU GENERAL PUBLIC LICENSE
 * Version 1, Mar 2019
 *
 * (C) 2019, Minghao Cheng <chengminghao92@gmail.com>
 */
#include "led_matrix.h"

LED_matrix* LED_matrix::addr = NULL;

/**
 * Contructor
 * 
 * @param The id of the thread
 * @return None
 */
LED_matrix::LED_matrix(pthread_t ID):Thread(ID)
{
    this->GPIO = new MCP23S17(LED_GPIO_SPI_CHANNEL);
    LED_matrix::addr = this;
}

/**
 * Destructor
 * 
 * @param None
 * @return None
 */
LED_matrix::~LED_matrix()
{
    delete this->GPIO;
}

/**
 * Initialise the hardware
 * 
 * @param None
 * @return result of the initialisation, 0 --> success, -1 --> failed
 */
int8_t LED_matrix::init(void)
{
    return this->GPIO->init();
}

/**
 * Write value of a row to the green led matrix
 * 
 * @param index of the row
 * @param value of the row
 * @return None
 */
void LED_matrix::write_green(uint8_t row, uint8_t val)
{
    this->v_buffer[row] &= 0xf0;
    this->v_buffer[row] |= (val&0xf);
}

/**
 * Write value of a row to the red led matrix
 * 
 * @param index of the row
 * @param value of the row
 * @return None
 */
void LED_matrix::write_red(uint8_t row, uint8_t val)
{
    this->v_buffer[row] &= 0x0f;
    this->v_buffer[row] |= (val&0xf)<<4;
}

/**
 * LED matrix refresh function
 * Refresh the physical LED matrix device, should be called every LED_ROW_REFRESH_PERIOD_US us
 * 
 * @param index of the row
 * @param value of the row
 * @return None
 */
void LED_matrix::refresh(void)
{
    static uint8_t row_index = 0;
    static bool refresh_flag = true;
    if(refresh_flag)
    {
        // write value to GPIOA,PNP transistors here
        this->GPIO->Set_GPIOA(
                                ~(
                                (1 << row_index)|(1 << (row_index+LED_MATRIX_ROW))
                                )
                             );
        // write value to GPIOB,NPN transistors here, row
        this->GPIO->Set_GPIOB(this->v_buffer[row_index]);
        // switch to next row
        row_index++;
        if(row_index == LED_MATRIX_ROW)
        {
            row_index = 0;
        }
    }
    else
    {
        this->GPIO->Set_GPIOA(0xff);
        this->GPIO->Set_GPIOB(0x00);
    }
    refresh_flag = !refresh_flag;
}

/**
 * Callback of the timer of the class
 * 
 * @param number sig
 * @param pointer to the siginfo_t
 * @param void pointer
 * @return None
 */
void LED_matrix::TIMER_handler(int sig, siginfo_t *si, void *uc)
{
    LED_matrix::addr->refresh();
    //if(si->si_value.sival_ptr == LED_matrix::addr)
    //{
    //    (reinterpret_cast<LED_matrix *> (si->si_value.sival_ptr))->refresh();
    //}
}

/**
 * Initialise the timer
 * Procedure:
 *        1. init the signal handler;
 *        2. init the sigqueue which would pass the address of the instance;
 *        3. start the ULARM timer
 * 
 * @param None
 * @return None
 */
void LED_matrix::thread_handler(void)
{
    // sigaction, allow user to pass param to handler
    struct sigaction sa;
    // custom param which would be passed to handler
    //union sigval sigval;
    // allow param to be passed
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = LED_matrix::TIMER_handler;
    // pass the pointer of this instance
    //sigval.sival_ptr = this;
    // block no signal
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGALRM);
    // register signal handler
    sigaction(SIGALRM, &sa, NULL);
    // pass param when signal happens
    //sigqueue(getpid(), SIGALRM, sigval);
    //signal(SIGALRM);
    ualarm(LED_ROW_REFRESH_PERIOD_US, LED_ROW_REFRESH_PERIOD_US);
    while(1)
    {
        sleep(100);
    }
}
