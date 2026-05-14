#include"Dictionary.hpp"
#include "DictServer.hpp"
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
        exit(0);
    }
    EnableConsoleLogStrategy();//配置日志输出策略
    
    uint16_t port = std::stoi(argv[1]);

    Dictionary dict("./dict.txt");


    
    std::unique_ptr<DictServer> usvr = std::make_unique<DictServer>(port,
        [&dict](const std::string &word,const std::string &whoip,uint16_t whoport)->std::string{
            return dict.Translate(word,whoip,whoport);
        }
    );
    usvr->Init();
    usvr->Start();

    return 0;
}