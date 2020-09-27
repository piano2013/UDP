#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_BUFF_LEN 500

typedef struct {
    unsigned int msgType;
    unsigned short msgLen;
    unsigned short TI;
}pkg_hdr_t;
#define PKG_HEADER_LEN			(sizeof(pkg_hdr_t))

typedef struct {
    pkg_hdr_t hdr;
    unsigned char buff[482];
}udpdata_pkg_t;
#define UDPDATA_PKG_LEN			(sizeof(udpdata_pkg_t))

typedef enum {
    BASE_STATION_ASK_KEEPALIVE = 0x01,
    BASE_STATION_REP_KEEPALIVE = 0x02,
    BASE_STATION_CONFIG = 0x05,
    BASE_STATION_REPORT_MIB = 0x06,
    BASE_STATION_REPORT_CINFO = 0x07,
    BASE_STATION_REPORT_PHSPS100 = 0x08,
    BASE_STATION_REPORT_PHSPL100 = 0x09,
    BASE_STATION_REPORT_CSTLTION = 0xa,
    BASE_STATION_RE_SEARCH = 0xb,
    BASE_STATION_SIB1 = 0xc,
    BASE_STATION_MSG3 = 0xd,
    BASE_STATION_MSG4 = 0xe,
    BASE_STATION_PAGEING = 0xf,
}pkg_e;

/*void udpsend(char* str)
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
        printf("Could not find a usable version of Winsock.dll\n");
        WSACleanup();
        return 1;
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
}*/
void udpSend(udpdata_pkg_t* data)
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
        printf("Could not find a usable version of Winsock.dll\n");
        WSACleanup();
        return 1;
    }
    else
        printf("The Winsock 1.1 dll was found okay\n");

    char msg[MAX_BUFF_LEN];
    int i_struct_member;
    //创建socket对象
    int sockClientfd = socket(AF_INET, SOCK_DGRAM, 0);

    //创建网络通信对象
    struct sockaddr_in addrServer;
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(4999);
    addrServer.sin_addr.s_addr = inet_addr("127.0.0.1");

    int sendFlag = 1;

    for (i_struct_member = 0; i_struct_member < 3; i_struct_member++)
    {
        if (0 == i_struct_member)
        {
            sprintf(msg, "%d", data->hdr.msgType);
            printf("%s\n", msg);
            sendFlag = sendto(sockClientfd, msg, strlen(msg) + 1, 0, (struct sockaddr*)&addrServer, sizeof(addrServer));
            if (sendFlag < 0)
                printf("Send Data_4RF Type failed!\n");
        }
        else if (1 == i_struct_member)
        {
            sprintf(msg, "%d", data->hdr.msgLen);
            printf("%s\n", msg);
            sendFlag = sendto(sockClientfd, msg, strlen(msg) + 1, 0, (struct sockaddr*)&addrServer, sizeof(addrServer));
            if (sendFlag < 0)
                printf("Send Data_4RF Length failed!\n");
        }
        else if (2 == i_struct_member)
        {
            for (int i = 0; i < data->hdr.msgLen; i++)
            {
                sprintf(msg, "%c", data->buff[i]);
                printf("%s ", msg); 
                sendFlag = sendto(sockClientfd, msg, strlen(msg) + 1, 0, (struct sockaddr*)&addrServer, sizeof(addrServer));
                if (sendFlag < 0)
                    printf("Send Data_4RF Content failed!\n");
            }                   
        }
    }

    closesocket(sockClientfd);
}

int main()
{
    int sibRdCnt = 34;
    unsigned char sib1[482];
    for (int i = 0; i < sibRdCnt * 4; i++)
        sib1[i] = i;
    udpdata_pkg_t* sib1_pkg = (udpdata_pkg_t*)malloc(sizeof(udpdata_pkg_t));
    sib1_pkg->hdr.msgType = BASE_STATION_SIB1;
    sib1_pkg->hdr.msgLen = (unsigned short)sibRdCnt * 4;
    memcpy(sib1_pkg->buff, sib1, sibRdCnt*4);
    //sib1_pkg->buff = sib1;
    for (int i = 0; i < sibRdCnt * 4; i++)
        printf("%c ", sib1_pkg->buff[i]);
    printf("\n");
    udpSend(sib1_pkg);
    printf("\n");
    printf("Send success!\n");
    return 0;
}