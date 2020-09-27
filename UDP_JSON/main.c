#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#include "cJSON.h"
#include "UDPserver.h"
#include "operate_json.h"

enum {
    BASE_STATION_SIB1 = 0xc,
    BASE_STATION_MSG3 = 0xd,
    BASE_STATION_MSG4 = 0xe,
    BASE_STATION_PAGEING = 0xf,
};

int main()
{
    //初始化
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
        return NULL;
    }
    else
        printf("The Winsock 1.1 dll was found okay\n");

    while (1) {
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
        if (len > 14) {
            for (int i = 10; i < 14; i++)
                numvalue = numvalue * 10 + buffer[i];

            printf("%x\n", numvalue);
        }
        cJSON* json_before = open_cfgfile("create.json");
        char* node = cJSON_Print(json_before);
        printf("modify before:");
        printf(node);
        printf("\n");

        cJSON* json_after = replace_value_from_cfgfile(json_before, "PCI", numvalue);
        char* node1 = cJSON_Print(json_after);
        printf("modify_:");
        printf(node1);
        printf("\n");
        cJSON* json_after1 = replace_value_from_cfgfile(json_after, "SCS", numvalue);
        char* node2 = cJSON_Print(json_after1);
        printf("modify2 after:");
        printf(node2);

        FILE* fp = fopen("create.json", "w+");
        char* buff = cJSON_Print(json_after1);
        printf("%d", strlen(buff));
        fwrite(buff, strlen(buff), 1, fp);
        //清理工作
        fclose(fp);
        close_cfgfile(json_before);
    }    
    return 0;
}