#include <stdio.h>
#include <wiringPi.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <string.h> 
#include <unistd.h>
#include "control.h"
#define PORT 3210 


void exit_handler(int signo)// function use ctrl + c to interrupt program
{
	stop();
	
	exit(0);
}

int dir;
void *keyboard(void *arg)// keyboard function use 1,2,3,4,5,6 to control car 
{
	while(1)
			switch(dir)
			{
				case 2:
					stop();
					break;
				case 1:
					forward();
					break;
				case 5:
					stop();
					break;
				case 3:
					turnleft();
					break;
				case 4:
					turnright();
					break;
				default:
					break;
			}	
}


int main(int argc, char const *argv[])// main function
{	
	wiringPiSetup();//set up gpio
	signal(SIGINT, exit_handler);  // ctrl+c to interrupt
	pinMode(0, OUTPUT);
	pinMode(2, OUTPUT);
	pinMode(21, OUTPUT);
	pinMode(22, OUTPUT);
	pinMode(23, OUTPUT);
	pinMode(24, OUTPUT);
	stop();
	pthread_t id;// call thread
	pthread_create(&id, NULL, keyboard, NULL);


	int i;
	struct sockaddr_in address; // use socket
       int sock = 0, valread; 
       struct sockaddr_in serv_addr; 
    	const char *hello = "Hello from client"; 
    	char buffer[1024] = {0}; 
    	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) //wrong creation 
    	{ 
        	printf("\n Socket creation error \n"); 
        	return -1; 
    	} 
   
    	memset(&serv_addr, '0', sizeof(serv_addr)); 
   
    	serv_addr.sin_family = AF_INET; 
    	serv_addr.sin_port = htons(PORT); 
       
    	// Convert IPv4 and IPv6 addresses from text to binary form 
    	if(inet_pton(AF_INET, "192.168.0.103", &serv_addr.sin_addr)<=0)  
    	{ 
        	printf("\nInvalid address/ Address not supported \n"); 
        	return -1; 
    	} 
   
    	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) 	< 0) 
    	{ 
        	printf("\nConnection Failed \n"); 
        	return -1; 
    	} 
    	send(sock , hello , strlen(hello) , 0 ); 
    	printf("Hello message sent\n"); 
    
	while(1)
		{
			valread = read( sock , buffer, 1024);
			//scanf("%d", &buffer[0]);
			if(valread>0)
			{
				printf("Byte received, val = %x\n",buffer[0]);
				dir = buffer[0];
			}
		}//while
		return 0;
	}//main
