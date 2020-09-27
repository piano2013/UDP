#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

enum {
    BASE_STATION_SIB1 = 0xabcdcccc,
    BASE_STATION_MSG3 = 0xd,
    BASE_STATION_MSG4 = 0xe,
    BASE_STATION_PAGEING = 0xf,
};

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

udpdata_pkg_t* udpReceive(){
    //创建socket对象
    int ASNsockSrvfd = socket(AF_INET, SOCK_DGRAM, 0);

    //创建网络通信对象
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(4999);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //绑定socket对象与通信链接
    int ret = bind(ASNsockSrvfd, (struct sockaddr*)&addr, sizeof(addr));
    if (0 > ret)
        printf("bind failed\n");

    struct sockaddr_in addrClient;
    socklen_t length = sizeof(addrClient);

    int receiveFlag = 1;
    char RecType[5];
    char RecLen[5];
    char RecBuff[5];

    int i_member;
    udpdata_pkg_t* rdata = (udpdata_pkg_t*)malloc(sizeof(udpdata_pkg_t));
    if (rdata == NULL)
        return NULL;
    else {
        printf("ASN_Receive start...\n");
        for (i_member = 0; i_member < 3; i_member++){
            if (0 == i_member){
                receiveFlag = recvfrom(ASNsockSrvfd, RecType, 5, 0, (struct sockaddr*)&addrClient, &length);
                if (receiveFlag < 0)
                    printf("Data_4RF Type Receive failed!\n");
                else {
                    unsigned int* read = (unsigned int*)RecType;
                    rdata->hdr.msgType = *read;
                }
                    
            }
            else if (1 == i_member){
                receiveFlag = recvfrom(ASNsockSrvfd, RecLen, 5, 0, (struct sockaddr*)&addrClient, &length);
                if (receiveFlag < 0)
                    printf("Data_4RF Length Receive failed!\n");
                else
                    rdata->hdr.msgLen = atoi(RecLen);
            }
            else if (2 == i_member){
                for (int i = 0; i < rdata->hdr.msgLen; i++){
                    receiveFlag = recvfrom(ASNsockSrvfd, RecBuff, 5, 0, (struct sockaddr*)&addrClient, &length);
                    if (receiveFlag < 0)
                        printf("Data_4RF Content Receive failed!\n");
                    else
                        rdata->buff[i] = atoi(RecBuff);
                }
            }
        }
        //shutdown(ASNsockSrvfd, 2);
        //close(ASNsockSrvfd);
        closesocket(ASNsockSrvfd);
        return rdata;
    }   
}
int main(){
    //初始化
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    wVersionRequested = MAKEWORD(1, 1);
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0){
        perror("WSAStartup error");
    }
    if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1) {
        printf("Could not find a usable version of Winsock.dll\n");
        WSACleanup();
        return NULL;
    }
    else
        printf("The Winsock 1.1 dll was found okay\n");

    while (1){
        int len = 0;
        int numvalue = 0;
        unsigned char buffer[482];
        udpdata_pkg_t* redata = (udpdata_pkg_t*)malloc(sizeof(udpdata_pkg_t));
        
        printf("\n");
        redata = udpReceive();
        printf("  Data type: %x\n", redata->hdr.msgType);
        len = redata->hdr.msgLen;
        printf("  Data length: %d\n", len);
        if (redata->hdr.msgLen == 0)
            break;
        memcpy(buffer, redata->buff, redata->hdr.msgLen);
        free(redata);
        printf("  Data content: ");
        for (int i = 0; i < len; i++)
            printf("%x ", buffer[i]);
        printf("\n");
        if (len > 14){
            for (int i = 10; i < 14; i++)
                numvalue = numvalue * 10 + buffer[i];

            printf("%x\n", numvalue);
        }      
    }
    return 0;
}