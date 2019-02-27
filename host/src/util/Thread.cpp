#include "Thread.h"

Thread::Thread(thread_func func, pthread_t ID)
{
    this->id = ID;
    this->pFunc = func;
}

Thread::~Thread()
{
    
}

void Thread::start(void)
{
    if(0 != pthread_create(&this->id, NULL, &Thread::exe, this))
    {
        //printf("thread create error\n");
    }
}
void Thread::join(void)
{
    pthread_join(this->id, NULL);
}

void *Thread::exe(void *param)
{
    reinterpret_cast<Thread *> (param)->pFunc();
    return NULL;
}
