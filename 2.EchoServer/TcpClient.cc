#include<iostream>
#include<string>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include"Comm.hpp"

void Usage(std::string proc)
{
    std::cerr << "Usage:" << proc << " serverip serverport" << std::endl;
}

int main(int argc,char *argv[])
{
    if(argc != 3)
    {
        Usage(argv[0]);
        exit(0);
    }

    std::string serverip = argv[1];
    uint16_t serverport = std::stoi(argv[2]);

    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd < 0)
    {
        std::cerr << "create client sockfd error";
        exit(SOCKET_CREATE_ERR);
    }

    if(connect(sockfd,) != 0)
    {
        std::cerr << "connect server error" << std::endl;
        exit(SOCKET_CONNECT_ERR);
    }

    return 0;
}