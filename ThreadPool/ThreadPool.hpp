#pragma once

#include <iostream>
#include <queue>
#include <vector>
#include <unistd.h>
#include <atomic>
#include "Logger.hpp"
#include "Mutex.hpp"
#include "Cond.hpp"
#include "Thread.hpp"

const static int defaultthreadnum = 3;

template <class T>
class ThreadPool
{
private:
    bool QueueIsEmpty()
    {
        return _q.empty();
    }
    void Routine(const std::string &name)
    {
        while (_is_running)
        {
            T t;
            {
                LockGuard lockguard(&_lock);
                while (QueueIsEmpty() && _is_running)
                {
                    _wait_thread_num++;
                    _cond.Wait(_lock);
                    _wait_thread_num--;
                }
                if (QueueIsEmpty()) break;  // 被唤醒时已停止，且无剩余任务
                t = _q.front();
                _q.pop();
            }
            t();
            LOG(LogLevel::DEBUG) << name << "handler task: " << t.Result2String();
        }
    }

public:
    ThreadPool(int threadnum = defaultthreadnum)
        : _threadnum(threadnum), _wait_thread_num(0)
    {
        for (int i = 0; i < _threadnum; i++)
        {
            // 方法1
            //  auto f = std::bind(hello,this);
            // 方法2
            std::string name = "thread-" + std::to_string(i + 1);
            _threads.emplace_back([this](const std::string &name)
                                  { this->Routine(name); }, name);
            // Thread t([this]() -> void
            //          { this->hello(); }, name);

            // _threads.push_back(t);
        }
        LOG(LogLevel::INFO) << "thread pool obj create success";
    }
    void Start()
    {
        if (_is_running)
            return;
        _is_running = true;
        for (auto &t : _threads)
        {
            t.Start();
        }
        LOG(LogLevel::INFO) << "thread pool running success";
    }
    void Stop()
    {
        if (!_is_running)
            return;
        _is_running = false;
        _cond.NotifyAll();
        LOG(LogLevel::INFO) << "thread pool stop success";
    }
    void Wait()
    {
        for (auto &t : _threads)
        {
            t.Join();
        }
        LOG(LogLevel::INFO) << "thread pool wait success";
    }
    void Enqueue(const T &t)
    {
        {
            LockGuard lockguard(&_lock);
            _q.push(t);
            if(_wait_thread_num > 0)
            {
                _cond.NotifyOne();
            }
        }
    }
    ~ThreadPool()
    {
    }

private:
    // 任务队列
    std::queue<T> _q; // 整体使用的临界资源

    // 多个线程
    std::vector<Thread> _threads; // 1.创建线程对象  2.让线程对象启动
    int _threadnum;
    int _wait_thread_num;

    // 保护机制
    Mutex _lock;
    Cond _cond;

    // 其他属性
    std::atomic<bool> _is_running{false};
};