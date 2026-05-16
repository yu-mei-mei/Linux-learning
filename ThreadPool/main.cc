#include"ThreadPool.hpp"
#include"Task.hpp"
#include<memory>
#include<time.h>



int main()
{
    srand(time(nullptr) ^ getpid());
    EnableConsoleLogStrategy();
    std::unique_ptr<ThreadPool<Task>> tp = std::make_unique<ThreadPool<Task>>();

    tp->Start();
    int cnt = 10;
    while(cnt)
    {
        //生产者生产任务
        int x = rand() % 10 + 1;
        usleep(rand()%73);
        int y = rand() % 5 + 1;
        Task t(x,y);
        //push 到线程中，处理
        tp->Enqueue(t);
        cnt--;
        sleep(1);
    }
    tp->Stop();
    tp->Wait();
    

    return 0;
}