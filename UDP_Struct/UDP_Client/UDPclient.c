#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

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

enum {
    BASE_STATION_SIB1 = (unsigned int)0xabcdcccc,
    BASE_STATION_MSG3 = (unsigned int)0xd,
    BASE_STATION_MSG4 = (unsigned int)0xe,
    BASE_STATION_PAGEING = (unsigned int)0xf,
};

void udpSend(udpdata_pkg_t* data){
    char msg[5];
    int i_struct_member;
    //创建socket对象
    int sockClientfd = socket(AF_INET, SOCK_DGRAM, 0);

    //创建网络通信对象
    struct sockaddr_in addrServer;
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(4999);
    addrServer.sin_addr.s_addr = inet_addr("127.0.0.1");

    int sendFlag = 1;

    for (i_struct_member = 0; i_struct_member < 3; i_struct_member++){
        if (0 == i_struct_member){
            //sprintf(msg, "%d", data->hdr.msgType);
            memcpy(msg, &(data->hdr.msgType), sizeof(data->hdr.msgType));
            msg[sizeof(data->hdr.msgType)] = '\0';
            //printf("  Data type: %s\n", msg);
            sendFlag = sendto(sockClientfd, msg, strlen(msg) + 1, 0, (struct sockaddr*)&addrServer, sizeof(addrServer));
            if (sendFlag < 0)
                printf("Send Data_4RF Type failed!\n");
        }
        else if (1 == i_struct_member){
            sprintf(msg, "%d", data->hdr.msgLen);
            //printf("  Data length: %s\n", msg);
            sendFlag = sendto(sockClientfd, msg, strlen(msg) + 1, 0, (struct sockaddr*)&addrServer, sizeof(addrServer));
            if (sendFlag < 0)
                printf("Send Data_4RF Length failed!\n");
        }
        else if (2 == i_struct_member){
            //printf("  Data content: ");
            for (int i = 0; i < data->hdr.msgLen; i++){
                sprintf(msg, "%d", data->buff[i]);
                //printf("%s ", msg); 
                sendFlag = sendto(sockClientfd, msg, strlen(msg) + 1, 0, (struct sockaddr*)&addrServer, sizeof(addrServer));
                if (sendFlag < 0)
                    printf("Send Data_4RF Content failed!\n");
            }                   
        }
    }
    //close(sockClientfd);
    closesocket(sockClientfd);
}

int main(){   
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    wVersionRequested = MAKEWORD(1, 1);
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        perror("WSAStartup error");
    }
    if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1) {
        printf("Could not find a usable version of Winsock.dll\n");
        WSACleanup();
    }

    int sibRdCnt = 0;
    
    
    while (1)
    {
        int ret = scanf("%d", &sibRdCnt);
        if (!ret || sibRdCnt >= 482 || sibRdCnt < 0)
            break;
        clock_t start, end;
        unsigned char sib1[482];  
        for (int i = 0; i < sibRdCnt; i++)
            sib1[i] = i;
        udpdata_pkg_t* sib1_pkg = (udpdata_pkg_t*)malloc(sizeof(udpdata_pkg_t));
        if (sib1_pkg == NULL)
            printf("sib1_pkg error!");
        else {
            sib1_pkg->hdr.msgType = BASE_STATION_SIB1;
            sib1_pkg->hdr.msgLen = (unsigned short)sibRdCnt;
            memcpy(sib1_pkg->buff, sib1, sibRdCnt);
            for (int i = 0; i < sibRdCnt; i++)
                printf("%x ", sib1_pkg->buff[i]);
        }
        printf("\n");

        printf("Send...\n");
        start = clock();
        udpSend(sib1_pkg);
        end = clock();
        printf("\n");
        printf("Send success!\n");
        printf("time = %3.2f ms\n", ((double)end - (double)start));
        free(sib1_pkg);
    }  
    return 0;
}