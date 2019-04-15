/*
 * GNU GENERAL PUBLIC LICENSE
 * Version 1, Mar 2019
 *
 * (C) 2019, Minghao Cheng <chengminghao92@gmail.com>
 */
#include "timer.h"

/**
 * Constructor
 * 
 * @param  None
 * @return None
 */
timer::timer()
{
    // We create a static handler catches the signal SIG
    this->sa.sa_flags = SA_SIGINFO;
    this->sa.sa_sigaction = timer::timer_handler;
    sigemptyset(&this->sa.sa_mask);
    if (sigaction(SIGRTMIN, &this->sa, NULL) == -1)
        throw("Could not create signal handler");
    
    // Create the timer
    this->sev.sigev_notify = SIGEV_SIGNAL;
    this->sev.sigev_signo = SIGRTMIN;
    // Cruical is that the signal carries the pointer to this class instance here
    // because the handler just handles anything that comes in!
    this->sev.sigev_value.sival_ptr = this;
    // create the timer
    if (timer_create(CLOCK_MONOTONIC, &this->sev, &this->timerid) == -1)
    {
        throw("Could not create timer");
    }
};

/**
 * Destructor
 * 
 * @param  None
 * @return None
 */
timer::~timer()
{
    // delete the timer
    timer_delete(this->timerid);
    // default action for signal handling
    signal(SIGRTMIN, SIG_IGN);
}

/**
 * Start the timer (signal)
 * 
 * @param  timer period, nanosecs
 * @return None
 */
void timer::start_timer(uint64_t nanosecs)
{
    // starts instantly
    this->its.it_value.tv_sec = 0;
    this->its.it_value.tv_nsec = 1;
    this->its.it_interval.tv_sec = nanosecs / 1000000000;
    this->its.it_interval.tv_nsec = nanosecs % 1000000000;
    if (timer_settime(this->timerid, 0, &this->its, NULL) == -1)
    {
        throw("Could not start timer");
    }
}

/**
 * Reinterpret cast to the member function
 * 
 * @param number sig
 * @param pointer to the siginfo_t
 * @param void pointer
 * @return None
 */
void timer::timer_handler(int sig, siginfo_t *si, void *uc)
{
    (reinterpret_cast<timer *> (si->si_value.sival_ptr))->timerEvent();
}
