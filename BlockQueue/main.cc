#include"BlockQueue.hpp"
#include"Task.hpp"
#include<unistd.h>


struct ThreadData
{
    BlockQueue<Task> *bq;
    std::string name;
};

void *consumer(void *args)
{
    ThreadData *td = static_cast<ThreadData *>(args);
    while(true)
    {
        
        Task t;
        td->bq->Pop(&t);
        t();
        t.Print();
    }
}

void *prodector(void *args)
{
    
    ThreadData *td = static_cast<ThreadData *>(args);
    int data= 1;
    while(true)
    {
        int x = data;
        int y = data+1;
        Task t(x,y);
        data++;
        //数据生产到阻塞独队列里
        td->bq->Enqueue(t);
        std::cout << "生产者生产了一个任务：" << data++ << std::endl;
    }
}

int main()
{
    // 线程间通信时，确实可以传递至整数，浮点数。字符串等
    //传递类对象、函数、可以给另一个线程派发问题吗？
    //创建阻塞队列
    BlockQueue<Task> *bq = new BlockQueue<Task>();
    pthread_t c,p;

    ThreadData ctd = {bq,"消费者"};
    pthread_create(&c,nullptr,consumer,(void*)&ctd);

    ThreadData ptd = {bq,"生产者"};
    pthread_create(&p,nullptr,prodector,(void*)&ptd);

    pthread_join(c,nullptr);
    pthread_join(p,nullptr);


    // pthread_t c[2],p[3];

    // ThreadData ctd = {bq,"消费者"};
    // pthread_create(c+0,nullptr,consumer,(void*)&ctd);//把阻塞队列传递给消费者线程
    // pthread_create(c+1,nullptr,consumer,(void*)&ctd);

    // ThreadData ptd = {bq,"生产者"};
    // pthread_create(p+0,nullptr,prodector,(void*)&ptd);//把阻塞线程传递给生产者线程，这里的阻塞队列是共享资源，需要我们加锁保护
    // pthread_create(p+1,nullptr,prodector,(void*)&ptd);
    // pthread_create(p+2,nullptr,prodector,(void*)&ptd);

    // //等待线程结束，回收线程资源
    // pthread_join(c[0],nullptr);
    // pthread_join(c[1],nullptr);
    // pthread_join(p[0],nullptr);
    // pthread_join(p[1],nullptr);
    // pthread_join(p[2],nullptr);
    //销毁阻塞队列
    delete bq;
    return 0;

}