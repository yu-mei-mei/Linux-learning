#pragma once 

#include<iostream>
#include<semaphore.h>

class Sem
{
public:
    Sem(int num):_initnum(num)
    {
        sem_init(&_sem,0,_initnum);
    }
    void P()
    {
        int n = sem_wait(&_sem);
        (void)n;
    }
    void V()
    {
        int n = sem_post(&_sem);
        (void)n;
    }
    ~Sem()
    {
        sem_destroy(&_sem);
    }
private:
    sem_t _sem;
    int _initnum;
};