#pragma comment(lib, "Ws2_32.lib")

#include <winsock2.h>
#include <stdio.h>

#define MAX_BUFF_LEN 256

int main()
{
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        /* Tell the user that we could not find a usable */
        /* Winsock DLL.                                  */
        printf("WSAStartup failed with error: %d\n", err);
        return 1;
    }

    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
        /* Tell the user that we could not find a usable */
        /* WinSock DLL.                                  */
        printf("Could not find a usable version of Winsock.dll\n");
        WSACleanup();
        return 1;
    }
    else
        printf("The Winsock 2.2 dll was found okay\n");

    SOCKET sockSrv = socket(AF_INET, SOCK_DGRAM, 0);

    SOCKADDR_IN  addrServ;
    addrServ.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    addrServ.sin_family = AF_INET;
    addrServ.sin_port = htons(4999);

    bind(sockSrv, (SOCKADDR*)&addrServ, sizeof(SOCKADDR));

    SOCKADDR_IN  addrClient;
    int length = sizeof(SOCKADDR);
    char recvBuf[MAX_BUFF_LEN];

    while (1)
    {
        int ret = recvfrom(sockSrv, recvBuf, MAX_BUFF_LEN, 0, (SOCKADDR*)&addrClient, &length);
        if (0 == strcmp(recvBuf, "Bye!"))
        {          
            printf("%s\n",recvBuf);

            if ((sendto(sockSrv, "Data Received!\n", strlen("Data Received!\n") + 1, 0, (SOCKADDR*)&addrClient, length)) < 0)
                printf("确认信号发送失败！\n");
            else
                printf("确认信号发出！\n");

            break;
        }         
        if (ret != -1)
        {
            printf("%s\n", recvBuf);
            if ((sendto(sockSrv, "Data Received!\n", strlen("Data Received!\n") + 1, 0, (SOCKADDR*)&addrClient, length)) < 0)
                printf("确认信号发送失败！\n");
            else
                printf("确认信号发出！\n");
        }         
        else
            printf("数据未送达！\n");
    }

    

    closesocket(sockSrv);

    WSACleanup();
    return 0;
}