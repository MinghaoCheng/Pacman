/*
 * GNU GENERAL PUBLIC LICENSE
 * Version 1, Mar 2019
 *
 * (C) 2019, Minghao Cheng <chengminghao92@gmail.com>
 */
#include "Thread.h"

/*
 * Description:
 * Constructor
 */
Thread::Thread()
{
    
}

/*
 * Description:
 * Constructor
 */
Thread::Thread(pthread_t ID)
{
    this->id = ID;
}

/*
 * Description:
 * Destructor
 */
Thread::~Thread()
{
    
}

/*
 * Description:
 * Create& start the thread
 */
void Thread::thread_start(void)
{
    if(0 != pthread_create(&this->id, NULL, &Thread::exe, this))
    {
        //printf("thread create error\n");
    }
}

/*
 * Description:
 * Thread join
 */
void Thread::thread_join(void)
{
    pthread_join(this->id, NULL);
}

/*
 * Description:
 * Real function of the thread, in this way it could be a non-static member function
 */
void Thread::thread_handler(void)
{
    
}

/*
 * Description:
 * Function of the thread, reinterpert the param(address of the instance) and run the real thread function
 */
void *Thread::exe(void *param)
{
    reinterpret_cast<Thread *> (param)->run();
    return NULL;
}
