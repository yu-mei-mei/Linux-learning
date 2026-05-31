
#include"Route.hpp"
#include"ThreadPool.hpp"
#include "ChatServer.hpp"
#include <iostream>
#include <memory>

void Usage(std::string proc)
{
    std::cerr << "Usage:" << proc << "  localport" << std::endl;
}

// void chat(int sockfd,std::string message,InetAddr addr){
//     LOG(LogLevel::DEBUG) << "sockfd: " << sockfd;
//     LOG(LogLevel::DEBUG) << "message: " << message;
//     LOG(LogLevel::DEBUG) << "client info: " << addr.ToString();
//     sendto(sockfd,message.c_str(),message.size(),0,addr.Addr(),addr.Length());
// }
using task_t = std::function<void()>;


int main(int argc,char *argv[])
{
    if(argc != 2)
    {
        Usage(argv[0]);
        exit(0);
    }
    EnableConsoleLogStrategy();//配置日志输出策略
    
    uint16_t port = std::stoi(argv[1]);

   //1.消息转发功能
   std::unique_ptr<Route> r = std::make_unique<Route>();
   
   //2.线程池对象
    ThreadPool<task_t> tp(3);
    tp.Start();

    //服务器对象
    std::unique_ptr<ChatServer> usvr = std::make_unique<ChatServer>(port,
        [&r,&tp](int sockfd,std::string message,InetAddr addr){
            task_t task = std::bind(&Route::RouteMessageToAll,r.get(),sockfd,std::ref(message),addr);
            tp.Enqueue(task);
        }
    );


    // std::unique_ptr<ChatServer> usvr = std::make_unique<ChatServer>(port,
    //     [](const std::string &word,const std::string &whoip,uint16_t whoport)->std::string{
          
    //     }
    // );

    // std::unique_ptr<ChatServer> usvr = std::make_unique<ChatServer>(port,chat);//?
    usvr->Init();
    usvr->Start();

    return 0;
}