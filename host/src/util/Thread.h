/*
 * GNU GENERAL PUBLIC LICENSE
 * Version 1, Mar 2019
 *
 * (C) 2019, Minghao Cheng <chengminghao92@gmail.com>
 */
#ifndef _THREAD_H_
#define _THREAD_H_

#include <stdint.h>
#include <pthread.h>

/**
 * Thread base function
 */
class Thread
{
    public:
    Thread();
    Thread(pthread_t ID);
    ~Thread();
    
    void thread_start(void);
    void thread_join(void);
    
    protected:
    virtual void thread_handler(void);
    
    private:
    pthread_t id;
    static void *exe(void *param);
};

#endif
