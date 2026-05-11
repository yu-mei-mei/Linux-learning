#pragma once

#include<iostream>
#include<vector>
#include"Sem.hpp"
#include"Mutex.hpp"

static int gcap = 5;

template <typename T>
class RingQueue
{
public:
    RingQueue(int cap = gcap):_cap(cap),_ring_queue(cap),_space_sem(cap),_data_sem(0),_p_step(0),_c_step(0)
    {}
    void Pop(T *out)
    {
        _c_lock.Lock();

        _data_sem.P();
        *out = _ring_queue[_c_step++];
        _c_step %= _cap; 
        _space_sem.V();

        _c_lock.Unlock();

    }
    void Enqueue(const T& in)
    {
        _space_sem.P();
        {
            LockGuard lockguard(&_p_lock);
        //有空间就可以生产数据，可是有空间在哪里呢？
        
        _ring_queue[_p_step++] = in;
        //维持环形特点
        _p_step %= _cap;
        }
        _data_sem.V();

        
    }
    ~RingQueue()
    {}
private:

    std::vector<T> _ring_queue;//临界资源（通过成员函数被外部线程访问）
    int _cap;


    Sem _space_sem;
    Sem _data_sem;

    //生产和消费的位置
    int _p_step;
    int _c_step;

    //两把锁
    Mutex _p_lock;
    Mutex _c_lock;

};