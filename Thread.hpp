#ifndef __THREAD_HPP__
#define __THREAD_HPP__

#include<iostream>
#include<pthread.h>
#include<unistd.h>
#include<string>
#include<functional>

#include<sys/syscall.h>

#define get_lwp_id() syscall(SYS_gettid)



using func_t = std::function<void()>;
const std::string threadnamedefault = "None-Name";

class Thread
{
public:
    Thread(func_t func,const std::string &name = threadnamedefault)
    :_name(name),
    _func(func),
    _isrunning(false)
    {

    }
   static void *start_routine(void *args)
    {
        Thread *self = static_cast<Thread*>(args);
        self->_isrunning = true;
        self->_lwpid = get_lwp_id();
        self->_func();
        pthread_exit(nullptr);
    }
    void Start()
    {
        if(_isrunning)
            return;
        
        int n = pthread_create(&_tid,nullptr,start_routine,this);
        if(n == 0)
        {
            std::cout << "create thread success" << std::endl;
        }
    }
    // void Die()
    // {
    //     pthread_cancel(_tid);
    // }
    //承担检验线程结束并且回收的功能
    void Join()
    {
        int n = pthread_join(_tid,nullptr);
        if(n == 0)
        {
            std::cout << "pthread_join success" << std::endl;
        }
    }
    ~Thread()
    {}
private:
    bool _isrunning;
    pthread_t _tid;
    pid_t _lwpid;
    std::string _name;
    func_t _func;
};
#endif