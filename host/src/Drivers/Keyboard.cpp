/*
 * GNU GENERAL PUBLIC LICENSE
 * Version 1, Mar 2019
 *
 * (C) 2019, Minghao Cheng <chengminghao92@gmail.com>
 */
 
#include "Keyboard.h"

Keyboard::Keyboard(pthread_t ID, callback *CB):Thread(ID)
{
    this->cb = CB;
}

Keyboard::~Keyboard()
{
    
}

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
