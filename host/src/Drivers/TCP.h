/*
 * GNU GENERAL PUBLIC LICENSE
 * Version 1, Mar 2019
 *
 * (C) 2019, Minghao Cheng <chengminghao92@gmail.com>
 */
#ifndef _TCP_H_
#define _TCP_H_

#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <string.h>

#include "brd_config.h"
#include "./../util/Thread.h"

/*
 * Description:
 * Class that create and maintain a TCP socket as server
 */
class TCP_dev:public Thread
{
    public:
    TCP_dev(pthread_t ID);
    virtual ~TCP_dev();
    int8_t init(void);
    int send_buffer(char* buffer, uint8_t size);
    
    private:
    void thread_handler(void);
    int server_fd, socket_handler;
    struct sockaddr_in address;
    bool Is_connected;
};


#endif
