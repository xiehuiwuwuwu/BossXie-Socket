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
#define MAX 4096
int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in ser_addr;
    char recvline[100],sendline[100];

    if (sockfd == -1)
    {
        std::cout << "create socket error" << std::endl;
        printf("error");
        return 0;
    }

    std::cout << "create successful" << std::endl;
    memset(&ser_addr,0,sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(7788);
    ser_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    //inet_pton(AF_INET,"192.168.0.2",&ser_addr.sin_addr);

    std::cout << "create successful" << std::endl;
    int temp = connect(sockfd,(const struct sockaddr*)&ser_addr,sizeof(ser_addr));
    std::cout << "create successful" << std::endl;

    if(temp < 0)
    {
        std::cout << "connect error" << std::endl;
	return 0;
    }
    
    std::cout << "Msg you can send:" << std::endl;
    //std::cin >> sendline;
    
    while(1)
    {
        std::cin >> sendline;
        if(sendline[0] == 'q' && sendline[1] == '\0')
        {
            break;
        }
        if (send(sockfd,(const void*)&sendline,strlen((const char*)&sendline),0) < 0)
        {
        std::cout << "send message error" << std::endl;
        return 0; 
        }
        memset(&sendline,0,sizeof(sendline));


    }
    
//    if (send(sockfd,(const void*)&sendline,strlen((const char*)&sendline),0) < 0)
 //   {
//	std::cout << "send message error" << std::endl;
//	return 0;
//    }
    close(sockfd);
    return 0;
}

