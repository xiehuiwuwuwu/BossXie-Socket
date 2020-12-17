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
class Service
{
public:
    Service();
    ~Service();
    void created_socket();
    void bind_socket();
    void listen_socket();
    void accept_socket();
    void receive_socket();
    void close_socket();
    void thread_listen();
private:
    int socketfd;
    int connfd;       //定义一个accept的句柄，用于recv函数的参数
    struct sockaddr_in ser_addr;
    char recvbuff[MAX];
    char sendbuff[MAX];
    int recvnum;    //创建recv句柄
    struct sockaddr_in client_addr;
    int socketlen;
    int reuse;
};

Service::Service()
{
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    memset((void*)&ser_addr,0,sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    ser_addr.sin_port = htons(7788);
    socketlen = sizeof(struct sockaddr_in);
    memset(recvbuff,0,sizeof(recvbuff));
    reuse = 1;
    setsockopt(socketfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
}

Service::~Service()
{

}

void Service::created_socket()
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

void Service::bind_socket()
{
    if(bind(socketfd, (struct sockaddr*)&ser_addr, sizeof(ser_addr)) == -1)
    {
        std::cout << "bind socket error" << std::endl;
    }
    else
    {
        std::cout << "bind socket success..." << std::endl;
    }
}

void Service::listen_socket()
{
    if(listen(socketfd,10) == -1)
    {
        std::cout << "failed to listen the socket" << std::endl;
    }
    else
    {
        std::cout << "listen socket success..." << std::endl;
    }
}

void Service::accept_socket()
{
    connfd = accept(socketfd,(struct sockaddr*)&client_addr,(socklen_t*)&socketlen);
    if(connfd == -1)
    {
        std::cout << "accept socket error" << std::endl;
    }
    else
    {
        std::cout << "accept socekt success" << std::endl;
    }
}

void Service::receive_socket()
{
    while(1)
    {
        recvnum = recv(connfd,recvbuff,MAX,0);
        if(recvnum > 0)
        {
            recvbuff[recvnum] = '\0';
            std::cout << recvbuff << std::endl;
            memset(recvbuff,0,sizeof(recvbuff));
        }
        else if(recvnum == 0)
        {
            break;
        }
    }

}

void thread_listen(Service s)
{
    while(1)
    {
        s.listen_socket();
        s.accept_socket();
        s.receive_socket();
    }

}

int main()
{
    Service socking;
    socking.created_socket();
    socking.bind_socket();
    std::thread t1(thread_listen,socking);
    std::thread t2(thread_listen,socking);
    std::thread t3(thread_listen,socking);
    t1.join();
    t2.join();
    t3.join();
}

