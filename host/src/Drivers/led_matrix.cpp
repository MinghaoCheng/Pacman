/*
 * GNU GENERAL PUBLIC LICENSE
 * Version 1, Mar 2019
 *
 * (C) 2019, Minghao Cheng <chengminghao92@gmail.com>
 */
#include "led_matrix.h"

/*
 * Description:
 * Constructor
 * Param:
 * The id of the thread
 */
LED_matrix::LED_matrix(pthread_t ID):Thread(ID)
{
    this->row = 1;
    this->GPIO = new MCP23S17(LED_GPIO_SPI_CHANNEL);
}

/*
 * Description:
 * Destructor
 */
LED_matrix::~LED_matrix()
{
    delete this->GPIO;
}

/*
 * Description:
 * Initialise the hardware
 */
int8_t LED_matrix::init(void)
{
    return this->GPIO->init();
}

/*
 * Description:
 * Write values of the led matrix
 * Param:
 * Values of rows, from row[0] to row[LED_MATRIX_ROW - 1], LED_MATRIX_COLUMN bits are valid in each row.
 */
void LED_matrix::write_val(uint8_t *row)
{
    uint8_t i;
    for (i=0; i<LED_MATRIX_ROW; i++)
    {
        this->v_buffer[i] = row[i];
    }
}

/*
 * Description:
 * Refresh the physical LED matrix device, should be called every LED_ROW_REFRESH_PERIOD_US us
 */
void LED_matrix::refresh(void)
{
    // write value to GPIO
    this->GPIO->Set_GPIOA(1 << this->row);
    // PNP transistors here
    this->GPIO->Set_GPIOB(~this->v_buffer[this->row]);
    // switch to next row
    this->row++;
    if(this->row == LED_MATRIX_ROW)
    {
        this->row = 0;
    }
}

/*
 * Description:
 * Callback of the timer of the class
 */
void LED_matrix::TIMER_handler(int sig, siginfo_t *si, void *uc)
{
    if(si != NULL)
    {
        (reinterpret_cast<LED_matrix *> (si->si_value.sival_ptr))->refresh();
    }
}

/*
 * Description:
 * Initialise the timer.
 * Note: the program would crash with "sigmentation error",
 *       which indicates there might be another program using ALARM signal and sigqueue
 * Procedure:
 *       1. init the signal handler;
 *       2. init the sigqueue which would pass the address of the instance;
 *       3. start the ULARM timer
 *
 */
void LED_matrix::run(void)
{
    // sigaction, allow user to pass param to handler
    struct sigaction sa;
    // custom param which would be passed to handler
    union sigval sigval;
    // allow param to be passed
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = LED_matrix::TIMER_handler;
    // pass the pointer of this instance
    sigval.sival_ptr = this;
    // block no signal
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGALRM);
    // register signal handler
    sigaction(SIGALRM, &sa, NULL);
    // pass param when signal happens
    sigqueue(getpid(), SIGALRM, sigval);
    //signal(SIGALRM);
    ualarm(LED_ROW_REFRESH_PERIOD_US, LED_ROW_REFRESH_PERIOD_US);
    while(1)
    {
        sleep(1);
    }
}

