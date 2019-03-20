#include "led_matrix.h"

LED_matrix::LED_matrix(pthread_t ID):Thread(ID)
{
    this->column = 0x01;
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
    for (i=0; i<LED_matrix_Row; i++)
    {
        this->v_buffer[i] = row[i];
    }
}

void LED_matrix::refresh(void)
{
    // write value to GPIO
    this->GPIO->Set_GPIOA(this->column);
    this->GPIO->Set_GPIOB(this->v_buffer[this->column]);
    // switch to next column
    this->column = this->column << 1;
    if(this->column == (0x01<<LED_matrix_Column))
    {
        this->column = 0x01;
    }
    printf("LED matrix: Refresh\n");
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
    ualarm(LED_row_refresh_period_us, LED_row_refresh_period_us);
    while(1)
    {
        sleep(1);
    }
}
