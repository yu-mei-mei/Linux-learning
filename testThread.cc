#include<iostream>
#include<vector>
#include"Thread.hpp"

void test()
{
    int cnt = 50;
    while(cnt--)
    {
        std::cout << "新线程运行了" << std::endl;
        sleep(1);
    }
}

int main()
{
    // //单个线程
    // Thread t(test,"thread-1");
    // t.Start();
    // t.Join();

    //多个线程还想管理：先描述再组织
    std::vector<Thread> threads;
    for(int i = 0;i < 5;i++)
    {
        std::string name = "thread-";
        name += std::to_string(i+1);
        Thread t(test,name);
        threads.push_back(t);
    }

    for(auto &thread : threads)
    {
        thread.Start();
    }
    for(auto &thread : threads)
    {
        thread.Join();
    }


    return 0;
}