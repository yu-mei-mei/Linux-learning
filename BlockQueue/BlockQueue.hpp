#pragma once
#include<iostream>
#include<string>
#include<queue>
#include<pthread.h>
#include<stdint.h>
#include<unistd.h>

const static uint32_t gcap = 5;

template<typename T>
class BlockQueue
{
private:
    bool IsFull()
    {
        return _bq.size() >= _cap;
    }
    bool IsEmpty()
    {
        return _bq.empty();
    }
public:
    BlockQueue(uint32_t cap = gcap):_cap(cap),_c_wait_num(0),_p_wait_num(0)
    {
        pthread_mutex_init(&_lock,nullptr);
        pthread_cond_init(&_c_cond,nullptr);
        pthread_cond_init(&_p_cond,nullptr);
    }
    void Enqueue(const T &in)
    {
        pthread_mutex_lock(&_lock);
        //要进行生产，就一定能够进行生产吗？还要满足生产条件
        while(IsFull())
        {
            //问题1 在进行等待的时候，我可是在临界区里等！当前是持有锁的！！需要让线程自动释放锁
            //问题2 为什么我要把自己弄得需要在临界区内等呢？？要先判断队列是否为满（生产条件是否满足）
            //判断队列是否为满本身就是在访问临界资源！！
            //生产者必须先申请锁，在临界区内部判断
            //判断为满的结果需要等待的结构也一定在临界区内部
            //所以，锁被做到pthread_cond_wait的参数当中！！
            //
            _p_wait_num++;
            pthread_cond_wait(&_p_cond,&_lock);//特征1：自动释放锁！   特征2：自动重新竞争并持有锁
            _p_wait_num--;
            //当我们被唤醒时，就一定又从这个位置唤醒了
            //是在临界区内被唤醒的
            sleep(1);
        }
        //不满
        _bq.push(in);//完成生产
        if(_c_wait_num>0)
            pthread_cond_signal(&_c_cond);//唤醒消费者
        pthread_mutex_unlock(&_lock);
    }
    void Pop(T *out)
    {
        pthread_mutex_lock(&_lock);
        while(IsEmpty())
        {
            _c_wait_num++;
            pthread_cond_wait(&_c_cond,&_lock);
            _c_wait_num--;
        }
        *out = _bq.front();
        _bq.pop();
        if(_p_wait_num>0)
            pthread_cond_signal(&_p_cond);//唤醒生产者
        pthread_mutex_unlock(&_lock);
        sleep(1);
    }
    ~BlockQueue()
    {
        pthread_mutex_destroy(&_lock);
        pthread_cond_destroy(&_c_cond);
        pthread_cond_destroy(&_p_cond);
    }
private:
    //临界资源
    std::queue<T> _bq;//blockqueue阻塞队列
    uint32_t _cap;//表示队列容量

    pthread_mutex_t _lock;
    pthread_cond_t _c_cond;//消费者用的cond
    pthread_cond_t _p_cond;//生产者用的cond


    int _c_wait_num;//当前消费者等待的个数
    int _p_wait_num;//当前生产者等待的个数

};