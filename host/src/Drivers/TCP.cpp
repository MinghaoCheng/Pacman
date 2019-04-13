/*
 * GNU GENERAL PUBLIC LICENSE
 * Version 1, Mar 2019
 *
 * (C) 2019, Minghao Cheng <chengminghao92@gmail.com>
 */
 
#include "TCP.h"
 
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
    char buffer[1024];
    int addrlen = sizeof(this->address);
    while(1)
    {
        if(!this->Is_connected)
        {
            printf("TCP: waiting for connection of Car\n");
            if((this->socket_handler = accept(this->server_fd, (struct sockaddr *)&this->address, (socklen_t*)&addrlen)) < 0)
            {
                printf("TCP: socket accept failed, check network connection\n");
            }
            else
            {
                this->Is_connected = true;
                printf("TCP: Car connected\n");
            }
        }
        if(0 < read(this->socket_handler, buffer, 1024))
        {
            //printf("%s\n", buffer);
        }
        else
        {
            printf("TCP: connection lost\n");
            this->Is_connected = false;
        }
    }
}
