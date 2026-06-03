#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void Usage(const std::string& proc)
{
    std::cerr << "Usage: " << proc << " serverip serverport" << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(0);
    }
    std::string serverip = argv[1];
    uint16_t serverport = std::stoi(argv[2]);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        std::cout << "create socket error" << std::endl;
        return 0;
    }

    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(serverport);
    server.sin_addr.s_addr = inet_addr(serverip.c_str());

    if (connect(sockfd, (struct sockaddr*)&server, sizeof(server)) < 0)
    {
        std::cout << "connect server error" << std::endl;
        close(sockfd);
        return 1;
    }

    while (true)
    {
        std::cout << "Please Enter@";
        std::string line;
        if (!std::getline(std::cin, line))
        {
            break;
        }

        ssize_t s = send(sockfd, line.c_str(), line.size(), 0);
        if (s <= 0)
        {
            std::cout << "send error" << std::endl;
            break;
        }

        char buffer[1024];
        ssize_t m = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
        if (m > 0)
        {
            buffer[m] = 0;
            std::cout << buffer << std::endl;
        }
        else if (m == 0)
        {
            std::cout << "server closed" << std::endl;
            break;
        }
        else
        {
            std::cout << "recv error" << std::endl;
            break;
        }
    }

    close(sockfd);
    return 0;
}
