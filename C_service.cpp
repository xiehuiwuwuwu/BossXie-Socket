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

class Service
{
public:
    Service();
    ~Service();
    void created_socket();            //成功创建套接字
    void bind_socket();               //成功绑定套接字
    void listen_socket();             //成功监听端口
    void accept_socket();             //成功接受连接数据
    int  receive_buff();              //执行接收数据
    void send_buff();                 //执行发送数据
    void add();
    int i;
    void close_socket();
private:
    int listenfd;                     //创建套接字
    int connfd;                       //创建accept句柄（应该是这么叫）
    struct sockaddr_in ser_addr;      //定义服务端地址类型
    char buff[MAX];                   //创建传输的数据内容
    int recvnum;                      //创建recv句柄
    struct sockaddr_in client_addr;     //定义客户端地址类型 置为NULL
    int socketlen;                    //定义ip地址结构体长度
    int reuse;
};

void Service::close_socket()
{
    close(listenfd);
}

Service::Service()
{
    listenfd = socket(AF_INET, SOCK_STREAM, 0);                //构造套接字
    memset((void*)&ser_addr,0,sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //ser_addr.sin_addr.s_addr = inet_addr("192.168.0.1");
    ser_addr.sin_port = htons(7788);
    socketlen = sizeof(struct sockaddr_in);
    memset(buff,0,sizeof(buff));
    i = 0;
    reuse = 1;
    setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
}

Service::~Service()
{
    close(listenfd);
}

void Service::created_socket()                //创建套接字
{
    if(listenfd == -1)
    {
        std::cout << "create socket error! " << std::endl;
    }
    std::cout << "create socket success..." << std::endl;
}

void Service::bind_socket()
{
    if(bind(listenfd, (struct sockaddr*)&ser_addr, sizeof(ser_addr)) == -1)
    {
        std::cout << "bind socket error" << std::endl;
    }
    std::cout << "bind socket success..." << std::endl;
}

void Service::listen_socket()
{
    if(listen(listenfd,10) == -1)
    {
        std::cout << "failed to listen the socket" << std::endl;
    }
    std::cout << "listen socket success..." << std::endl;
}

void Service::accept_socket()
{
    connfd = accept(listenfd,(struct sockaddr*)&client_addr,(socklen_t*)&socketlen);
    if(connfd == -1)
    {
        std::cout << "accept socket error" << std::endl;
    }
}

int Service::receive_buff()
{
    recvnum = recv(connfd,buff,MAX,0);
 //   if(recv(connfd,buff,MAX,0) > 0)
 //   {
 //       buff[recvnum] = '\0';
 //       std::cout << buff << std::endl;
 //   }
 //   memset(buff,0,sizeof(buff));
    std::cout << "recvnum = " << recvnum << std::endl;
    return recvnum;
}

void Service::add()
{
    i = Service::receive_buff();
    if(i > 0)
    {
        buff[recvnum] = '\0';
        std::cout << buff << std::endl;
        memset(buff,0,sizeof(buff));
    }
}

int main()
{
    int c = 1;
    while(1)
    {
        Service socking;
        socking.created_socket();
        socking.bind_socket();
        socking.listen_socket();
        socking.accept_socket();
        while(1)
        {
            socking.i = 1;
            socking.add();
            if(socking.i == 0)
            {
                c++;
                if(c > 5)
                {
                    socking.close_socket();
                    break;
                }
                continue;
            }
        }

    }
}
