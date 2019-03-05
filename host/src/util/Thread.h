#ifndef _THREAD_H_
#define _THREAD_H_

#include <stdint.h>
#include <pthread.h>

class Thread
{
    public:
    Thread(pthread_t ID);
    ~Thread();
    
    void start(void);
    void join(void);
    virtual void run(void);
    
    private:
    pthread_t id;
    static void *exe(void *param);
};

#endif
