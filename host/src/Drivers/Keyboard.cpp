/*
 * GNU GENERAL PUBLIC LICENSE
 * Version 1, Mar 2019
 *
 * (C) 2019, Minghao Cheng <chengminghao92@gmail.com>
 */
 
#include "Keyboard.h"

/**
 * Constructor, register the callback function
 * 
 * @param  ID of the thread
 * @param  pointer to the registering callback funtion
 * @return None
 */
Keyboard::Keyboard(pthread_t ID, callback *CB):Thread(ID)
{
    this->cb = CB;
}

/**
 * Destructor
 * 
 * @param  None
 * @return None
 */
Keyboard::~Keyboard()
{
    
}

/**
 * Initialisation, open the device file
 * 
 * @param  None
 * @return result, 0 --> success, -1 --> failed
 */
int8_t Keyboard::init(void)
{
    this->fds.fd = open("/dev/Pacman_dev", O_RDWR,S_IRUSR | S_IWUSR);
    if(this->fds.fd == -1)
    {
        printf("Keybord: Pac_dev file open failed\n");
        return -1;
    }
    return 0;
}

/**
 * Thread handler
 * Poll the device file, call the callback function when interrupt triggered
 * 
 * @param  None
 * @return None
 */
void Keyboard::thread_handler(void)
{
    char status, val[2];
    int poll_ret;
    
    this->fds.events = POLLRDBAND;
    while(1)
    {
        poll_ret = poll(&this->fds, 1, -1);
        if(poll_ret != 0)
        {
            status = read(this->fds.fd, val, 2);
            if(status)
            {
                this->cb->cb_func((uint8_t *)val, 1);
            }
        }
    }
}
