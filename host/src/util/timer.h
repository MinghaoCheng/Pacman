/*
 * GNU GENERAL PUBLIC LICENSE
 * Version 1, Mar 2019
 *
 * (C) 2019, Minghao Cheng <chengminghao92@gmail.com>
 * 
 * This is inspired by Dr.Bernd Porr
 * 
 */

#ifndef _TIMER_H_
#define _TIMER_H_

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>

class timer
{
    public:
	timer();    
	virtual ~timer();
	void start_timer(uint32_t nanosecs);
	virtual void timerEvent() = 0;
    private:
	timer_t timerid;
	struct sigevent sev;
	struct sigaction sa;
	struct itimerspec its;

	static void timer_handler(int sig, siginfo_t *si, void *uc );
};


#endif
