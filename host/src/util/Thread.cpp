/*
 * GNU GENERAL PUBLIC LICENSE
 * Version 1, Mar 2019
 *
 * (C) 2019, Minghao Cheng <chengminghao92@gmail.com>
 */
#include "Thread.h"

/**
 * Constructor
 * 
 * @param  None
 * @return None
 */
Thread::Thread()
{
    
}

/**
 * Constructor
 * 
 * @param  ID of the thread
 * @return None
 */
Thread::Thread(pthread_t ID)
{
    this->id = ID;
}

/**
 * Destructor
 * 
 * @param  None
 * @return None
 */
Thread::~Thread()
{
    
}

/**
 * Create& start the thread
 * 
 * @param  None
 * @return None
 */
void Thread::thread_start(void)
{
    if(0 != pthread_create(&this->id, NULL, &Thread::exe, this))
    {
        //printf("thread create error\n");
    }
}

/**
 * Thread join
 * 
 * @param  None
 * @return None
 */
void Thread::thread_join(void)
{
    pthread_join(this->id, NULL);
}

/**
 * Real function of the thread, in this way it could be a non-static member function
 * 
 * @param  None
 * @return None
 */
void Thread::thread_handler(void)
{
    
}

/**
 * Function of the thread, reinterpert the param(address of the instance) and run the real thread function
 * 
 * @param  param
 * @return None
 */
void *Thread::exe(void *param)
{
    reinterpret_cast<Thread *> (param)->thread_handler();
    return NULL;
}
