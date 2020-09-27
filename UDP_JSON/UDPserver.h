#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

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

udpdata_pkg_t* udpReceive();