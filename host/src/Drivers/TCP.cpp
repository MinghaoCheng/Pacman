/*
 * GNU GENERAL PUBLIC LICENSE
 * Version 1, Mar 2019
 *
 * (C) 2019, Minghao Cheng <chengminghao92@gmail.com>
 */
 
#include "TCP.h"
#include <errno.h>
TCP_dev::TCP_dev(pthread_t ID):Thread(ID)
{
    
}

TCP_dev::~TCP_dev()
{
    
}

int8_t TCP_dev::init(void)
{
    int opt = 1;
    //struct timeval timeout;
    //timeout.tv_sec = 1;
    //timeout.tv_usec = 0;
    // Creating socket file descriptor
    if((this->server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        printf("TCP: socket failed\n");
        return -1;
    }
    // Forcefully attaching socket to the port
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        printf("TCP: socket opt setting failed\n");
        return -1;
    }
    // Setting writing timeout
    //if(setsockopt(server_fd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)))
    //{
        //printf("TCP: socket opt setting failed\n");
        //return -1;
    //}
    this->address.sin_family = AF_INET;
    this->address.sin_addr.s_addr = INADDR_ANY;
    this->address.sin_port = htons(TCP_SOCKET);
    // Forcefully attaching socket to the port
    if(bind(this->server_fd, (struct sockaddr *)&this->address, sizeof(this->address))<0) 
    { 
        printf("TCP: socket binding failed\n");
        return -1;
    } 
    if(listen(this->server_fd, 3) < 0)
    { 
        printf("TCP: socket listening failed\n");
        return -1;
    }
    return 0;
}

int TCP_dev::send_buffer(char* buffer, uint8_t size)
{
    if(this->Is_connected)
    {
        return send(this->socket_handler, buffer, size, 0);
    }
    return -1;
}

void TCP_dev::thread_handler(void)
{
    struct pollfd fds;
    int rc;
    char buffer[10];
    int addrlen = sizeof(this->address);
    fds.events = POLLIN | POLLHUP | POLLRDNORM;
    while(1)
    {
        if(!this->Is_connected)
        {
            if((this->socket_handler = accept(this->server_fd, (struct sockaddr *)&this->address, (socklen_t*)&addrlen)) < 0)
            {
                if(errno != EINTR)
                {
                    printf("TCP: socket accept failed, check network connection\n");
                    //printf("TCP: %s\n", strerror(errno));
                }
            }
            else
            {
                fds.fd = this->socket_handler;
                this->Is_connected = true;
                printf("TCP: Car connected\n");
            }
        }
        else
        {
            rc = poll(&fds, 1, -1);
            if(rc > 0)
            {
                if(0 == recv(fds.fd, buffer, 10, 0))
                {
                    printf("TCP: Car disconnected\n");
                    this->Is_connected = false;
                }
                else
                {
                    printf("TCP: data received\n");
                }
            }
        }
    }
}
