#include "led_matrix.h"

LED_matrix::LED_matrix(pthread_t ID):Thread(ID)
{
    this->row = 1;
    this->GPIO = new MCP23S17(LED_GPIO_SPI_CHANNEL);
}

LED_matrix::~LED_matrix()
{
    delete this->GPIO;
}

int8_t LED_matrix::init(void)
{
    return this->GPIO->init();
}

void LED_matrix::write_val(uint8_t *row)
{
    uint8_t i;
    for (i=0; i<LED_MATRIX_ROW; i++)
    {
        this->v_buffer[i] = row[i];
    }
}

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

void LED_matrix::TIMER_handler(int sig, siginfo_t *si, void *uc)
{
    if(si != NULL)
    {
        (reinterpret_cast<LED_matrix *> (si->si_value.sival_ptr))->refresh();
    }
}

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

