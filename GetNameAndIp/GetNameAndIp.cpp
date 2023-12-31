﻿// GetNameAndIp.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

// copy from  https ://docs.microsoft.com/en-us/windows/win32/api/iphlpapi/nf-iphlpapi-sendarp

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <iostream>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

//void usage(char *pname)
//{
//    printf("Usage: %s [options] ip-address\n", pname);
//    printf("\t -h \t\thelp\n");
//    printf("\t -l length \tMAC physical address length to set\n");
//    printf("\t -s src-ip \tsource IP address\n");
//    exit(1);
//}

int __cdecl main(int argc, char **argv)
{
    DWORD dwRetVal;
    IPAddr DestIp = 0;
    IPAddr SrcIp = 0;       /* default for src ip */
    ULONG MacAddr[2];       /* for 6-byte hardware addresses */
    ULONG PhysAddrLen = 6;  /* default to length of six bytes */

    //char *DestIpString = NULL;
    //char *SrcIpString = NULL;
    std::string SrcIpString = "192.168.2.3";
    //std::string DestIpString = "192.168.2.13";
    std::string DestIpString = "192.168.2.3";

    BYTE *bPhysAddr;
    unsigned int i;

    if (DestIpString.empty() || DestIpString.empty()) {
        std::cout << "(DestIpString.empty() || DestIpString.empty()" << std::endl;
    }

    DestIp = inet_addr(DestIpString.c_str());

    memset(&MacAddr, 0xff, sizeof(MacAddr));

    std::cout << "Sending ARP request for IP address: " << DestIpString << std::endl;

    dwRetVal = SendARP(DestIp, SrcIp, &MacAddr, &PhysAddrLen);

    if (dwRetVal == NO_ERROR) {
        bPhysAddr = (BYTE *)& MacAddr;
        if (PhysAddrLen) {
            for (i = 0; i < (int)PhysAddrLen; i++) { // 打印Mac地址
                if (i == (PhysAddrLen - 1))
                    printf("%.2X\n", (int)bPhysAddr[i]);
                else {
                    printf("%.2X-", (int)bPhysAddr[i]);
                }
            }
        }
        else {
            std::cout << "Warning: SendArp completed successfully, but returned length=0" << std::endl;
        }
    }
    else {
        printf("Error: SendArp failed with error: %d", dwRetVal);
        switch (dwRetVal) {
        case ERROR_GEN_FAILURE:
            printf(" (ERROR_GEN_FAILURE)\n");
            break;
        case ERROR_INVALID_PARAMETER:
            printf(" (ERROR_INVALID_PARAMETER)\n");
            break;
        case ERROR_INVALID_USER_BUFFER:
            printf(" (ERROR_INVALID_USER_BUFFER)\n");
            break;
        case ERROR_BAD_NET_NAME:
            printf(" (ERROR_GEN_FAILURE)\n");
            break;
        case ERROR_BUFFER_OVERFLOW:
            printf(" (ERROR_BUFFER_OVERFLOW)\n");
            break;
        case ERROR_NOT_FOUND:
            printf(" (ERROR_NOT_FOUND)\n");
            break;
        default:
            printf("\n");
            break;
        }
    }

    return 0;
}




