#include "UdpServer.hpp"
#include <iostream>
#include <memory>

void Usage(std::string proc)
{
    std::cerr << "Usage:" << proc << "  localport" << std::endl;
}

int main(int argc,char *argv[])
{
    if(argc != 2)
    {
        Usage(argv[0]);
        exit(1);
    }
    std::string ip = argv[1];
    uint16_t port = std::stoi(argv[1]);

    EnableConsoleLogStrategy();//配置日志输出策略
    std::unique_ptr<UdpServer> usvr = std::make_unique<UdpServer>(port);
    usvr->Init();
    usvr->Start();

    return 0;
}