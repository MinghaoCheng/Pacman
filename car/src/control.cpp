#include <stdio.h>
#include <wiringPi.h>
#include <signal.h>
#include <stdlib.h>



void changespeed(int pin, int speed)// change speed fucntion  
{					
	int i;
	for(i=0; i<20; i++)
	{  //  use function x/1000 to change speed within 1000
		digitalWrite(pin, HIGH);
		delayMicroseconds(speed);
		digitalWrite(pin, LOW);
		delayMicroseconds(1000-speed);
	}
}

void forward()// forward function
{
	digitalWrite(21, LOW); 
	digitalWrite(22, HIGH); 
	digitalWrite(23, HIGH); 
	digitalWrite(24, LOW); 
	
	changespeed(0, 900);
	changespeed(2, 900);
}


void backward()// backward function
{
	digitalWrite(21, LOW); 
	digitalWrite(22, HIGH); 
	digitalWrite(23, HIGH); 
	digitalWrite(24, LOW); 
	
	changespeed(0, 900);
	changespeed(2, 900);
}



void turnright()// turn right function
{
	digitalWrite(21, LOW); 
	digitalWrite(22, HIGH); 
	
	digitalWrite(23, LOW); 
	digitalWrite(24, HIGH); 
	
	changespeed(0, 900);
	//changespeed(2, 900);
}

void turnleft()// turn left function
{
	digitalWrite(21, HIGH); 
	digitalWrite(22, LOW); 
	
	digitalWrite(23, HIGH); 
	digitalWrite(24, LOW); 
	
	//changespeed(0, 900);
	changespeed(2, 900);
}


void stop()// stop function 
{
	digitalWrite(21, LOW); 
	digitalWrite(22, LOW); 
	digitalWrite(23, LOW); 
	digitalWrite(24, LOW); 
	digitalWrite(0, LOW); 
	digitalWrite(2, LOW); 
}
