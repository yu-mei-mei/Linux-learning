#include"RingQueue.hpp"
#include<pthread.h>
#include<unistd.h>


void *consumer(void *args)
{
    RingQueue<int> *rq = static_cast<RingQueue<int> *>(args);
    while(true)
    {
        
        int data = 0;
        rq->Pop(&data);
        std::cout << "消费了一个数据" << data << std::endl;
    }
}
void *producer(void *args)
{
    RingQueue<int> *rq = static_cast<RingQueue<int> *>(args);
    int data = 1;
    while(true)
    {
        sleep(1);
        rq->Enqueue(data);
        
        std::cout << "生产了一个数据" << data << std::endl;
        data++;
    }
}

int main()
{
    RingQueue<int> *rq = new RingQueue<int>();
    pthread_t c[2],p[3];

    pthread_create(c,nullptr,consumer,(void*)rq);
    pthread_create(c+1,nullptr,consumer,(void*)rq);
    pthread_create(p,nullptr,producer,(void*)rq);
    pthread_create(p+1,nullptr,producer,(void*)rq);
    pthread_create(p+2,nullptr,producer,(void*)rq);


    pthread_join(c[0],nullptr);
    pthread_join(c[1],nullptr);
    pthread_join(p[0],nullptr);
    pthread_join(p[1],nullptr);
    pthread_join(p[2],nullptr);

    return 0;
}