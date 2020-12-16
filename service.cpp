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

int main()
{
    int listenfd,connfd;
    struct sockaddr_in ser_addr;
    char buff[MAX];
    int n;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1)
    {
        std::cout << "create socket error" << std::endl;
		//printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }
    std::cout << "create successful" << std::endl;    
    memset(&ser_addr,0,sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	//ser_addr.sin_addr.s_addr = inet_addr("192.168.0.1");
    ser_addr.sin_port = htons(7788);

    if(bind(listenfd, (struct sockaddr*)&ser_addr, sizeof(ser_addr)) == -1)
    {
	std::cout << "bind socket error" << std::endl;
	return 0;
    }

    if(listen(listenfd,10) == -1)
    {
	std::cout << "failed to listen the socket" << std::endl;
	return 0;
    }
    std::cout << "listen success" << std::endl;
    int socketlen = sizeof(struct sockaddr_in);
    struct sockaddr_in client_addr;
    
    if( (connfd = accept(listenfd,(struct sockaddr*)&client_addr,(socklen_t*)&socketlen)) == -1)   
    {
        std::cout << "accept socket error" << std::endl;
    }


    while(1)
    {
//	if( (connfd = accept(listenfd,(struct sockaddr*)&client_addr,(socklen_t*)&socketlen)) == -1)
//	{
//		std::cout << "accept socket error" << std::endl;
//		continue;
//	}
        
	n = recv(connfd,buff,MAX,0);
	
        if(n > 0)
        {
            buff[n] = '\0';
            std::cout << buff << std::endl;
        }
        
	//close(connfd);
	memset(&buff,0,sizeof(buff));
    }
    close(listenfd);
    return 0;
}


