#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include <stdio.h>
#include <unistd.h>
#define MAX_BUFF_LEN 256
void udpsend(char* str)
{   
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    wVersionRequested = MAKEWORD(1, 1);
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0)
    {
        perror("WSAStartup error");
    }
    if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1) {
        /* Tell the user that we could not find a usable */
        /* WinSock DLL.                                  */
        printf("Could not find a usable version of Winsock.dll\n");
        WSACleanup();
    }
    else
        printf("The Winsock 1.1 dll was found okay\n");

    //创建socket对象
    int sockClientfd = socket(AF_INET, SOCK_DGRAM, 0);

    //创建网络通信对象
    struct sockaddr_in addrServer;
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(4999);
    addrServer.sin_addr.s_addr = inet_addr("127.0.0.1");
    int length = sizeof(addrServer);

    int sendFlag = sendto(sockClientfd, str, strlen(str) + 1, 0, (struct sockaddr*)&addrServer, length);
    if (sendFlag != -1)
        printf("数据已发送！\n");
    
    char recvBuf[MAX_BUFF_LEN];

    int receiveFlag = recvfrom(sockClientfd, recvBuf, MAX_BUFF_LEN, 0, (struct sockaddr*)&addrServer, &length);
    if (receiveFlag < 0)
        printf("确认数据未收到！\n");
    else
        printf("%s\n", recvBuf);

    closesocket(sockClientfd);
    WSACleanup();
}

int main()
{
    char *str = "Hello C!";
    
    udpsend(str);
        
    return 0;
}