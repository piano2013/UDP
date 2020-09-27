#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include <stdio.h>

#define MAX_BUFF_LEN 256

void udpSend(char *str)
{
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested = MAKEWORD(1, 1);
    
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        /* Tell the user that we could not find a usable */
        /* Winsock DLL.                                  */
        printf("WSAStartup failed with error: %d\n", err);
        return 1;
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


    SOCKET sockClient = socket(AF_INET, SOCK_DGRAM, 0);

    SOCKADDR_IN  addrServ;
    addrServ.sin_addr.S_un.S_addr = inet_addr("192.168.199.229");
    addrServ.sin_family = AF_INET;
    addrServ.sin_port = htons(4999);

    int sendFlag = sendto(sockClient, str, strlen(str) + 1, 0, (SOCKADDR*)&addrServ, sizeof(SOCKADDR));
    if (sendFlag != -1)
        printf("数据已发送！\n");

    int length = sizeof(SOCKADDR);
    char recvBuf[MAX_BUFF_LEN];

    int receiveFlag = recvfrom(sockClient, recvBuf, MAX_BUFF_LEN, 0, (SOCKADDR*)&addrServ, &length);
    if (receiveFlag < 0)
        printf("确认数据未收到！\n");
    else
        printf("%s\n", recvBuf);

    closesocket(sockClient);
    WSACleanup();
}

int main()
{
    char *str = "Hello!";
    udpSend(str);
    return 0;
}