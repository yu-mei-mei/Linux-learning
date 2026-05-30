// // 文件名：sem_demo1.cpp
// #include <pthread.h>
// #include <semaphore.h>
// #include <unistd.h>
// #include <iostream>

// sem_t sem;  // 全局信号量

// // 线程1：先等待信号量
// void* worker(void* arg) {
//     std::cout << "线程1：准备等待信号量..." << std::endl;

//     // 如果信号量当前值为 0，这里会阻塞
//     sem_wait(&sem);

//     std::cout << "线程1：收到信号量，继续执行！" << std::endl;
//     return nullptr;
// }

// // 线程2：延迟一会儿，再释放信号量
// void* notifier(void* arg) {
//     std::cout << "线程2：先睡眠 2 秒..." << std::endl;
//     sleep(2);

//     std::cout << "线程2：准备释放信号量" << std::endl;

//     // 信号量 +1，并唤醒一个等待线程
//     sem_post(&sem);

//     std::cout << "线程2：已经释放信号量" << std::endl;
//     return nullptr;
// }

// int main() {
//     pthread_t t1, t2;

//     // 初始化信号量
//     // 第二个参数 0 表示线程间共享（不是进程间共享）
//     // 第三个参数 0 表示初始值为 0
//     sem_init(&sem, 0, 0);

//     pthread_create(&t1, nullptr, worker, nullptr);
//     pthread_create(&t2, nullptr, notifier, nullptr);

//     pthread_join(t1, nullptr);
//     pthread_join(t2, nullptr);

//     sem_destroy(&sem);

//     std::cout << "主线程退出" << std::endl;
//     return 0;
// }

// 文件名：sem_demo2.cpp
// #include <pthread.h>
// #include <semaphore.h>
// #include <unistd.h>
// #include <iostream>

// sem_t sem;  // 信号量，表示可用资源个数

// struct ThreadArg {
//     int id;
// };

// void* worker(void* arg) {
//     ThreadArg* info = static_cast<ThreadArg*>(arg);

//     std::cout << "线程 " << info->id << " 想进入临界资源区" << std::endl;

//     // 申请一个资源
//     sem_wait(&sem);

//     std::cout << "线程 " << info->id << " 已进入资源区，开始工作..." << std::endl;

//     // 模拟工作 3 秒
//     sleep(3);

//     std::cout << "线程 " << info->id << " 工作完成，离开资源区" << std::endl;

//     // 释放一个资源
//     sem_post(&sem);

//     return nullptr;
// }

// int main() {
//     pthread_t threads[5];
//     ThreadArg args[5];

//     // 初始化信号量为 2，表示同时最多允许 2 个线程进入
//     sem_init(&sem, 0, 2);

//     for (int i = 0; i < 5; ++i) {
//         args[i].id = i + 1;
//         pthread_create(&threads[i], nullptr, worker, &args[i]);
//     }

//     for (int i = 0; i < 5; ++i) {
//         pthread_join(threads[i], nullptr);
//     }

//     sem_destroy(&sem);

//     std::cout << "所有线程执行完毕，主线程退出" << std::endl;
//     return 0;
// }



// #include <pthread.h>
// #include <semaphore.h>
// #include <unistd.h>
// #include <iostream>

// const int N = 5;
// int buffer[N];
// int head = 0;
// int tail = 0;

// pthread_mutex_t mutex;
// sem_t emptySlots;
// sem_t fullSlots;

// void* producer(void*) {
//     for (int x = 1; x <= 10; ++x) {
//         sem_wait(&emptySlots);

//         pthread_mutex_lock(&mutex);
//         buffer[tail] = x;
//         std::cout << "produce " << x << " at " << tail << std::endl;
//         tail = (tail + 1) % N;
//         pthread_mutex_unlock(&mutex);

//         sem_post(&fullSlots);
//         usleep(200000);
//     }
//     return nullptr;
// }

// void* consumer(void*) {
//     for (int i = 1; i <= 10; ++i) {
//         sem_wait(&fullSlots);

//         pthread_mutex_lock(&mutex);
//         int x = buffer[head];
//         std::cout << "consume " << x << " at " << head << std::endl;
//         head = (head + 1) % N;
//         pthread_mutex_unlock(&mutex);

//         sem_post(&emptySlots);
//         usleep(300000);
//     }
//     return nullptr;
// }

// int main() {
//     pthread_t p, c;

//     pthread_mutex_init(&mutex, nullptr);
//     sem_init(&emptySlots, 0, N);
//     sem_init(&fullSlots, 0, 0);

//     pthread_create(&p, nullptr, producer, nullptr);
//     pthread_create(&c, nullptr, consumer, nullptr);

//     pthread_join(p, nullptr);
//     pthread_join(c, nullptr);

//     sem_destroy(&emptySlots);
//     sem_destroy(&fullSlots);
//     pthread_mutex_destroy(&mutex);
// }

// #include <pthread.h>
// #include <semaphore.h>
// #include <unistd.h>
// #include <iostream>

// const int N = 5;
// int buffer[N];
// int head = 0;
// int tail = 0;

// pthread_mutex_t producerMutex;
// pthread_mutex_t consumerMutex;
// sem_t emptySlots;
// sem_t fullSlots;

// void* producer(void* arg) {
//     int id = *static_cast<int*>(arg);

//     for (int i = 1; i <= 5; ++i) {
//         int value = id * 100 + i;

//         sem_wait(&emptySlots);

//         pthread_mutex_lock(&producerMutex);
//         buffer[tail] = value;
//         std::cout << "producer " << id
//                   << " put " << value
//                   << " at " << tail << std::endl;
//         tail = (tail + 1) % N;
//         pthread_mutex_unlock(&producerMutex);

//         sem_post(&fullSlots);
//         usleep(200000);
//     }

//     return nullptr;
// }

// void* consumer(void* arg) {
//     int id = *static_cast<int*>(arg);

//     for (int i = 1; i <= 5; ++i) {
//         sem_wait(&fullSlots);

//         pthread_mutex_lock(&consumerMutex);
//         int value = buffer[head];
//         std::cout << "consumer " << id
//                   << " get " << value
//                   << " at " << head << std::endl;
//         head = (head + 1) % N;
//         pthread_mutex_unlock(&consumerMutex);

//         sem_post(&emptySlots);
//         usleep(300000);
//     }

//     return nullptr;
// }

// int main() {
//     pthread_t p1, p2, c1, c2;
//     int id1 = 1, id2 = 2;

//     pthread_mutex_init(&producerMutex, nullptr);
//     pthread_mutex_init(&consumerMutex, nullptr);
//     sem_init(&emptySlots, 0, N);
//     sem_init(&fullSlots, 0, 0);

//     pthread_create(&p1, nullptr, producer, &id1);
//     pthread_create(&p2, nullptr, producer, &id2);
//     pthread_create(&c1, nullptr, consumer, &id1);
//     pthread_create(&c2, nullptr, consumer, &id2);

//     pthread_join(p1, nullptr);
//     pthread_join(p2, nullptr);
//     pthread_join(c1, nullptr);
//     pthread_join(c2, nullptr);

//     sem_destroy(&emptySlots);
//     sem_destroy(&fullSlots);
//     pthread_mutex_destroy(&producerMutex);
//     pthread_mutex_destroy(&consumerMutex);
// }

// #include<pthread.h>
// #include<unistd.h>
// #include<queue>
// #include<iostream>

// struct Task{
//     int id;
// };

// std::queue<Task> taskQueue;

// pthread_mutex_t mutex;
// pthread_cond_t cond;

// bool stop = false;

// void* worker(void *arg){
//     //取出当前工作线程的编号，方便打印观察
//     int workerId = *static_cast<int*>(arg);

//     //线程池里的工作线程不是干一次就退出，而是一直循环
//     //1.等任务   2.取任务   3，执行任务   4.再继续等下应该任务
//     while(true){
//         //先加锁，因为接下来要访问共享任务队列
//         pthread_mutex_lock(&mutex);
//         //如果当前任务队列为空，并且线程池还没有关闭
//         //当前工作线程就不能继续往下执行，只能等待
//         while(taskQueue.empty() && !stop){
//             std::cout << "工作线程" << workerId << "没有任务，开始等待" << std::endl;
//             //wait会做两件事：
//             //1是让当前线程进入睡眠等待状态
//             //2是自动释放Mutex，让别的线程继续往附列里放任务
//             pthread_cond_wait(&cond,&mutex);
//         }
//         //走到这里两种可能：被唤醒后，队列里已经有任务了，或者线程池被通知关闭了
//         if(stop && taskQueue.empty()){
//             pthread_mutex_unlock(&mutex);
//             std::cout << "工作线程" << workerId << "退出" << std::endl;
//             break;
//         }
//         //走到这里，说明当前队列里至少有一个任务可以取
//         Task task = taskQueue.front();
//         taskQueue.pop();

//         //任务已经从共享队列里取出来了，后面执行任务不需要访问共享队列，所以立刻解锁
//         pthread_mutex_unlock(&mutex);
//         //模拟执行任务
//         std::cout << "工作系统" << workerId << "正在执行任务" << task.id << std::endl;
//         //用sleep/usleep模拟任务耗时
//         usleep(500000);
//     }
//     return nullptr;
// }

// int main()
// {
//     //线程池里固定有三个工作线程
//     const int THREAD_COUNT = 3;
//     pthread_t threads[THREAD_COUNT];
//     int ids[THREAD_COUNT];

//     pthread_mutex_init(&mutex,nullptr);
//     pthread_cond_init(&cond,nullptr);

//     for(int i = 0;i < THREAD_COUNT;++i){
//         ids[i] = i+1;
//         pthread_create(&threads[i],nullptr,worker,&ids[i]);
//     }

//     //主线程模拟提交任务
//     for(int i = 1;i <= 6;++i){
//         pthread_mutex_lock(&mutex);

//         Task task;
//         task.id = i;
//         taskQueue.push(task);

//         std::cout << "主线程提交任务" << task.id << std::endl;

//         //放完任务后解锁
//         pthread_mutex_unlock(&mutex);

//         pthread_cond_signal(&cond);
//         usleep(200000);
//     }

//     //主线程先等一会，让工作线程有时间把任务处理完
//     sleep(2);

//     //开始关闭线程池
//     pthread_mutex_lock(&mutex);
//     stop = true;
//     pthread_mutex_unlock(&mutex);

//     pthread_cond_broadcast(&cond);
//     for(int i = 0;i < THREAD_COUNT;++i){
//         pthread_join(threads[i],nullptr);
//     }

//     pthread_mutex_destroy(&mutex);
//     pthread_cond_destroy(&cond);

//     std::cout << "主线程退出" << std::endl;
// }

// 文件名：thread_pool_demo2.cpp
#include <pthread.h>
#include <unistd.h>
#include <queue>
#include <vector>
#include <iostream>

struct Task {
    int id;
};

class ThreadPool {
public:
    // 构造函数：创建指定数量的工作线程
    ThreadPool(int threadCount)
        : stop_(false), threadCount_(threadCount) {
        pthread_mutex_init(&mutex_, nullptr);
        pthread_cond_init(&cond_, nullptr);

        threads_.resize(threadCount_);
        threadIds_.resize(threadCount_);

        for (int i = 0; i < threadCount_; ++i) {
            threadIds_[i] = i + 1;
            pthread_create(&threads_[i], nullptr, workerEntry, this);
        }
    }

    // 析构函数：关闭线程池并回收线程
    ~ThreadPool() {
        stop();
        for (int i = 0; i < threadCount_; ++i) {
            pthread_join(threads_[i], nullptr);
        }

        pthread_mutex_destroy(&mutex_);
        pthread_cond_destroy(&cond_);
    }

    // 提交一个任务到任务队列
    void pushTask(const Task& task) {
        pthread_mutex_lock(&mutex_);
        taskQueue_.push(task);
        pthread_mutex_unlock(&mutex_);

        // 新任务到来，唤醒一个等待中的工作线程
        pthread_cond_signal(&cond_);
    }

    // 关闭线程池
    void stop() {
        pthread_mutex_lock(&mutex_);
        stop_ = true;
        pthread_mutex_unlock(&mutex_);

        // 唤醒所有等待线程，让它们检查 stop_ 后退出
        pthread_cond_broadcast(&cond_);
    }

private:
    // 静态入口函数：pthread_create 只能接收普通函数指针
    static void* workerEntry(void* arg) {
        ThreadPool* pool = static_cast<ThreadPool*>(arg);
        pool->workerLoop();
        return nullptr;
    }

    // 真正的工作线程循环
    void workerLoop() {
        while (true) {
            pthread_mutex_lock(&mutex_);

            // 没有任务且线程池未关闭，就等待
            while (taskQueue_.empty() && !stop_) {
                pthread_cond_wait(&cond_, &mutex_);
            }

            // 如果线程池关闭且没有任务，退出线程
            if (stop_ && taskQueue_.empty()) {
                pthread_mutex_unlock(&mutex_);
                break;
            }

            // 取一个任务
            Task task = taskQueue_.front();
            taskQueue_.pop();

            pthread_mutex_unlock(&mutex_);

            // 执行任务
            std::cout << "线程 " << pthread_self()
                      << " 正在执行任务 " << task.id << std::endl;

            usleep(500000);
        }
    }

private:
    std::queue<Task> taskQueue_;        // 任务队列
    std::vector<pthread_t> threads_;    // 工作线程
    std::vector<int> threadIds_;        // 这里其实没真正用上，可以后面删掉

    pthread_mutex_t mutex_;             // 保护任务队列
    pthread_cond_t cond_;               // 队列为空时用于等待

    bool stop_;                         // 是否关闭线程池
    int threadCount_;                   // 工作线程数量
};

int main() {
    ThreadPool pool(3);

    for (int i = 1; i <= 6; ++i) {
        Task task;
        task.id = i;

        std::cout << "主线程提交任务 " << task.id << std::endl;
        pool.pushTask(task);

        usleep(200000);
    }

    sleep(3);
    return 0;
}