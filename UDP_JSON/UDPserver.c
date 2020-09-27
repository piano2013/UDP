#include <stdio.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#include "UDPserver.h"

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
                else
                    rdata->hdr.msgType = atoi(RecType);
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