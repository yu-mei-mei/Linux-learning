// #include<pthread.h>
// #include<unistd.h>
// #include<iostream>

// void* printNumbers(void *arg){
//     const char* threadName = static_cast<const char*>(arg);
//     for(int i = 1;i <= 5;i++){
//         std::cout << threadName << "打印数字" << i << std::endl;

//         usleep(200000);
//     }
//     return nullptr;
// }
// void* printLetters(void* arg) {
//     const char* threadName = static_cast<const char*>(arg);

//     for (char c = 'A'; c <= 'E'; ++c) {
//         std::cout << threadName << " 打印字母: " << c << std::endl;

//         // 暂停 300ms
//         usleep(300000);
//     }

//     return nullptr;
// }

// int main()
// {
//     //线程变量，用来存储线程ID
//     pthread_t thread1;
//     pthread_t thread2;

//     //给线程传的参数
//     const char* name1 = "线程1";
//     const char* name2 = "线程2";

//     //创建线程1
//     int ret1 = pthread_create(&thread1,nullptr,printNumbers,(void*)name1);
//     if(ret1 != 0){
//         std::cout << "创建线程1失败" << std::endl;
//         return 1;
//     }
//     //创建线程2
//     int ret2 = pthread_create(&thread1,nullptr,printLetters,(void*)name2);
//     if(ret2 != 0){
//         std::cout << "创建线程2失败" << std::endl;
//         return 1;
//     }

//     //主线程等待线程1结束
//     pthread_join(thread1, nullptr);
//     //主线程等待线程2结束
//     pthread_join(thread2, nullptr);


//     std::cout << "两个线程都执行完毕，主线程退出" << std::endl;
//     return 0;

// }

// // 文件名：race_demo.cpp
// #include <pthread.h>
// #include <iostream>

// const int TIMES = 1000000;   // 每个线程自增的次数

// long long counter = 0;       // 两个线程共享的全局变量
// pthread_mutex_t mutex;       // 互斥锁

// // 不加锁：存在竞态条件
// void* unsafeIncrement(void* arg) {
//     const char* name = static_cast<const char*>(arg);

//     for (int i = 0; i < TIMES; ++i) {
//         counter++;
//     }

//     std::cout << name << " 执行结束（不加锁）" << std::endl;
//     return nullptr;
// }

// // 加锁：保护共享资源
// void* safeIncrement(void* arg) {
//     const char* name = static_cast<const char*>(arg);

//     for (int i = 0; i < TIMES; ++i) {
//         pthread_mutex_lock(&mutex);
//         counter++;
//         pthread_mutex_unlock(&mutex);
//     }

//     std::cout << name << " 执行结束（加锁）" << std::endl;
//     return nullptr;
// }

// int main() {
//     pthread_t t1, t2;

//     std::cout << "===== 第一部分：不加锁 =====" << std::endl;
//     counter = 0;

//     const char* name1 = "线程1";
//     const char* name2 = "线程2";

//     pthread_create(&t1, nullptr, unsafeIncrement, (void*)name1);
//     pthread_create(&t2, nullptr, unsafeIncrement, (void*)name2);

//     pthread_join(t1, nullptr);
//     pthread_join(t2, nullptr);

//     std::cout << "理论结果: " << 2LL * TIMES << std::endl;
//     std::cout << "实际结果: " << counter << std::endl;

//     std::cout << std::endl;
//     std::cout << "===== 第二部分：加锁 =====" << std::endl;
//     counter = 0;

//     pthread_mutex_init(&mutex, nullptr);

//     pthread_create(&t1, nullptr, safeIncrement, (void*)name1);
//     pthread_create(&t2, nullptr, safeIncrement, (void*)name2);

//     pthread_join(t1, nullptr);
//     pthread_join(t2, nullptr);

//     std::cout << "理论结果: " << 2LL * TIMES << std::endl;
//     std::cout << "实际结果: " << counter << std::endl;

//     pthread_mutex_destroy(&mutex);

//     return 0;
// }

// 文件名：producer_consumer.cpp
#include <pthread.h>
#include <unistd.h>
#include <queue>
#include <iostream>

std::queue<int> buffer;          // 共享缓冲区
const int MAX_SIZE = 5;          // 缓冲区最大容量

pthread_mutex_t mutex;           // 保护共享队列
pthread_cond_t notEmpty;         // 队列非空条件
pthread_cond_t notFull;          // 队列非满条件

// 生产者线程：生产数据并放入缓冲区
void* producer(void* arg) {
    const char* name = static_cast<const char*>(arg);

    for (int i = 1; i <= 10; ++i) {
        pthread_mutex_lock(&mutex);

        // 如果缓冲区满了，生产者等待
        while (buffer.size() == MAX_SIZE) {
            std::cout << name << " 发现缓冲区已满，开始等待..." << std::endl;
            pthread_cond_wait(&notFull, &mutex);
        }

        buffer.push(i);
        std::cout << name << " 生产了数据: " << i
                  << "，当前缓冲区大小: " << buffer.size() << std::endl;

        // 通知消费者：队列里有数据了
        pthread_cond_signal(&notEmpty);

        pthread_mutex_unlock(&mutex);

        usleep(200000);
    }

    return nullptr;
}

// 消费者线程：从缓冲区取数据并消费
void* consumer(void* arg) {
    const char* name = static_cast<const char*>(arg);

    for (int i = 1; i <= 10; ++i) {
        pthread_mutex_lock(&mutex);

        // 如果缓冲区为空，消费者等待
        while (buffer.empty()) {
            std::cout << name << " 发现缓冲区为空，开始等待..." << std::endl;
            pthread_cond_wait(&notEmpty, &mutex);
        }

        int value = buffer.front();
        buffer.pop();

        std::cout << name << " 消费了数据: " << value
                  << "，当前缓冲区大小: " << buffer.size() << std::endl;

        // 通知生产者：队列有空位了
        pthread_cond_signal(&notFull);

        pthread_mutex_unlock(&mutex);

        usleep(300000);
    }

    return nullptr;
}

int main() {
    pthread_t prodThread;
    pthread_t consThread;

    const char* producerName = "生产者";
    const char* consumerName = "消费者";

    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&notEmpty, nullptr);
    pthread_cond_init(&notFull, nullptr);

    pthread_create(&prodThread, nullptr, producer, (void*)producerName);
    pthread_create(&consThread, nullptr, consumer, (void*)consumerName);

    pthread_join(prodThread, nullptr);
    pthread_join(consThread, nullptr);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&notEmpty);
    pthread_cond_destroy(&notFull);

    std::cout << "生产和消费全部完成，主线程退出" << std::endl;
    return 0;
}