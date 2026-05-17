#include"TcpEchoServer.hpp"
#include<memory>

int main()
{
    EnableConsoleLogStrategy();//打印到显示器上

    std::unique_ptr<TcpEchoServer> tsvr = std::make_unique<TcpEchoServer>();

    tsvr->Init();
    tsvr->Start();

    return 0;
}