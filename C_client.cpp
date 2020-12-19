#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <thread>
#define MAX 4096
using namespace std;
class Client
{
public:
    Client();
    ~Client();

    char* get_sendbuff();
    void created_socket();
    int connect_socket();
    int send_socket(int,char*);
    void close_socket();
    int get_sockfd();
private:
    int sendfd;
    int socketfd;
    struct sockaddr_in ser_addr;
    char recvbuff[MAX];
    char sendbuff[MAX];
    int connfd;;        //定义connect句柄
};

Client::Client()
{
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    memset((void*)&ser_addr,0,sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    ser_addr.sin_port = htons(7788);
    memset(sendbuff,0,sizeof(sendbuff));
}

Client::~Client()
{

}

void Client::created_socket()
{
    if(socketfd == -1)
    {
        std::cout << "create socket error! " << std::endl;
    }
    else
    {
        std::cout << "create socket success..." << std::endl;
    }
}

int Client::connect_socket()
{
    connfd = connect(socketfd,(const struct sockaddr*)&ser_addr,sizeof(ser_addr));
    if (connfd < 0)
    {
        std::cout << "connect socket error" << std::endl;
        return -1;
    }
    else
    {
        std::cout << "connect socket success " << std::endl;
        return 0;
    }
}

int Client::send_socket(int sockfd,char* sendline)
{
    sendfd = send(sockfd,(const void*)sendline,strlen((const char*)sendline),0);
    if (sendfd < 0)
    {
        std::cout << "send message failed..." << std::endl;
        return -1;
    }
}

char* Client::get_sendbuff()
{
    return sendbuff;
}

void Client::close_socket()
{
    close(socketfd);
}

int Client::get_sockfd()
{
    return socketfd;
}

int main()
{
    char* psend;
    Client socking;
    psend = socking.get_sendbuff();
    socking.created_socket();
    if (socking.connect_socket() == -1)
    {
        return 0;
    }
    while(1)
    {
        std::cin >> psend;

        if (psend[0] == 'q' && psend[1] == '\0')
        {
            break;
        }
        if (socking.send_socket(socking.get_sockfd(),psend) == -1)
        {
            std::cout << "send messages failed..." << std::endl;
            return 0;
        }
        //memset(psend,0,sizeof(sendbuff));
    }
    socking.close_socket();
}

