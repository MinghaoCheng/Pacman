#ifndef _THREAD_H_
#define _THREAD_H_

#include <stdint.h>
#include <pthread.h>

typedef void (*thread_func) (void);

class Thread
{
    public:
    Thread(thread_func func, pthread_t ID);
    ~Thread();
    
    void start();
    void join();
    
    private:
    pthread_t id;
    thread_func pFunc;
    static void *exe(void *param);
    /*
    {
        reinterpret_cast<Thread *> (param)->pFunc();
        return NULL;
    }
    */
};

#endif
