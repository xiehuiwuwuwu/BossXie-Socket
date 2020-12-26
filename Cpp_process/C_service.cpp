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
#include <mutex>
#include <pthread.h>
#include <vector>
#include <unistd.h>
#include <algorithm>
#include <map>
#include <set>
#define MAX 4096
using namespace std;
std::set<int> clientfd;     //全局的set容器clientfd，存放不同客户端的connfd（句柄）
std::mutex mtx;             //多线程的锁
class Service               //服务端类
{
public:
    Service();
    ~Service();
    void   created_socket();            //创建socket套接字
    void   bind_socket();               //绑定服务器端口用于监听和接收message
    void   listen_socket();             //监听端口
    void   accept_socket();             //接受客户端连接
    void   receive_socket();            //接收message
    void   close_socket();              //关闭套接字
private:
    int    socketfd;                    //定义套接字
    int    connfd;                      //定义一个accept的句柄，用于recv函数的参数
    struct sockaddr_in ser_addr;        //定义服务端地址的结构体
    char   recvbuff[MAX];               //接收的数据
    int    recvnum;                     //创建recv句柄
    struct sockaddr_in client_addr;     //定义客户端地址的结构体
    int    socketlen;                   //定义结构体长度（这个定义有点重复了）
    int    reuse;                       //定义端口重用
};


Service::Service()
{
    socketfd = socket(AF_INET, SOCK_STREAM, 0);                             //套接字
    memset((void*)&ser_addr,0,sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;                                          //定义服务端地址
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    ser_addr.sin_port = htons(7788);
    socketlen = sizeof(struct sockaddr_in);
    memset(recvbuff,0,sizeof(recvbuff));
    reuse = 1;
    setsockopt(socketfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));      //使端口可重用
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
        std::cout << "connfd is " << connfd << std::endl;
        std::cout << "accept socekt success" << std::endl;
    }
    mtx.lock();                                            //多线程的锁，避免多线程同时操作变量
    clientfd.insert(connfd);
    mtx.unlock();
}

void Service::receive_socket()
{
    while(1)
    {
        recvnum = recv(connfd,recvbuff,MAX,0);
        if(recvnum > 0)
        {
            set<int> i = clientfd;
            recvbuff[recvnum] = '\0';
            std::cout << recvbuff << std::endl;
            if (i.size() > 0)
            {
                for (auto iter = i.begin();iter != i.end();++iter)
                {
                    if (*iter == connfd)
                    {
                        continue;
                    }
                    send(*iter,recvbuff,sizeof(recvbuff),0);
                }
            }
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

