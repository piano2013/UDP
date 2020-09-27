#include <iostream>
#include <WinSock2.h>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

#define MAX_BUFF_LEN 256

int main()
{
    //初始化
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
        return 1;
    }
    else
        printf("The Winsock 1.1 dll was found okay\n");

    //创建socket对象
    int sockSrvfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    //创建网络通信对象
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(4999);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //绑定socket对象与通信链接
    int ret = bind(sockSrvfd, (struct sockaddr*)&addr, sizeof(addr));
    if (ret < 0) {
        cout << "bind failed\n" << endl;
        return 1;
    }

    struct sockaddr_in addrClient;
    int length = sizeof(addrClient);
    char RecBuff[MAX_BUFF_LEN];

    int RecFlag = recvfrom(sockSrvfd, RecBuff, MAX_BUFF_LEN, 0, (struct sockaddr*)&addrClient, &length);
    /*if (0 == strcmp(RecBuff, "Bye!"))
    {
        printf("%s\n", RecBuff);

        if ((sendto(sockSrvfd, "Data Received!\n", strlen("Data Received!\n") + 1, 0, (struct sockaddr*)&addrClient, length)) < 0)
            printf("确认信号发送失败！\n");
        else
            printf("确认信号发出！\n");

        break;
    }*/
    if (RecFlag != -1)
    {
        printf("%s\n", RecBuff);
        if ((sendto(sockSrvfd, "Data Received!\n", strlen("Data Received!\n") + 1, 0, (struct sockaddr*)&addrClient, length)) < 0)
            printf("确认信号发送失败！\n");
        else
            printf("确认信号发出！\n");
    }
    else
        printf("数据未送达！\n");

    //关闭socket连接
    closesocket(sockSrvfd);

    //清理
    WSACleanup();
    return 0;
}