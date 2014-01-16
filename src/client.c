/* 
 * Simple Wake-on-LAN utility.
 */

#include <stdio.h> /* printf */
#include <stdlib.h> /* exit */
#include <string.h> /* memset & memcpy */
#include <sys/socket.h> /* socket, sendto, setsockopt */
#include <arpa/inet.h> /* htonl & htons */
#include <errno.h> /* errno */
#include <assert.h> /* assert */
#include <unistd.h> /* close, getopt */


int sendWol(const char *macAddrHex, unsigned port);

int main(int argc, char *const argv[])
{
    if (sendWol("00:e0:4c:02:c5:bb", 9999) < 0) {
        printf("Could not send WoL frame\n");
    }
    return 0;
}

static int macTranslate(const char *macAddrHex, unsigned char *macAddrInt);
static int strHexToInt(const char *str, size_t strLen, unsigned *iOut);
static int hexToInt(char c);

int sendWol(const char *macAddrHex, unsigned port)
{
    assert(macAddrHex != NULL);

    unsigned char macAddr[7];
    unsigned char message[102];
    unsigned char *messagePtr = message;
    int sock;
    int optval = 1;
    struct sockaddr_in sAddr;
    
    /* Fetch MAC adress */
    if (macTranslate(macAddrHex, macAddr) < 0) {
        printf("Specified MAC adress is not valid!\n");
        return -1;
    }

    if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        perror("ERROR socket");
        return -1;
    }

    /* Build Wake on LAN UDP segment */
    memset(messagePtr, 0xFF, 6); // 6 x 0xFF
    messagePtr += 6;
    for (int i = 0; i < 16; ++i) { // 16 x MACaddr
        memcpy(messagePtr, macAddr, 6);
        messagePtr += 6;
    }

    /* Allow socket sending broadcast messages */
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)) < 0) {
        perror("ERROR setsockopt");
        close(sock);
        return -1;
    }

    /* Set up broadcast address */
    sAddr.sin_family = AF_INET;
    sAddr.sin_addr.s_addr = inet_addr("192.168.0.255");
    sAddr.sin_port = htons(port);

    /* Send */
    if (sendto(sock, (char *)message, sizeof(message), 0, (struct sockaddr *)&sAddr, sizeof(sAddr)) < 0) {
        perror("ERROR sendto");
        close(sock);
        return -1;
    }

    close(sock);
    printf("WoL frame succesfuly send to %s\n", macAddrHex);
    return 0;
}


/*  Translate MAC adress from standard format 
    XX:XX:XX:XX:XX:XX or XXXXXXXXXXXX
    to string of unsigned chars IIIIII */
static int macTranslate(const char *macAddrHex, unsigned char *macAddrInt)
{
    assert(macAddrHex != NULL);
    assert(macAddrInt != NULL);

    const char *origMacAddrHex = macAddrHex;
    unsigned iOut = 0;
    for (size_t i = 0; i < 6; ++i) {
        int readChars = strHexToInt(macAddrHex, 2, &iOut);
        if (readChars < 0) {
            return -1;
        }

        /* Append new integer representing two hex digits */
        *macAddrInt++ = (unsigned char)iOut;
        macAddrHex += readChars;
        /* Skip ':' if present */
        if (*macAddrHex == ':') ++macAddrHex;
    }

    return (macAddrHex - origMacAddrHex);
}

/* Translate string of hex values to one integer */
static int strHexToInt(const char *str, size_t strLen, unsigned *iOut) 
{
    assert(str != NULL);
    assert(iOut != NULL);

    unsigned iTmp;
    for (int i = 0; i < strLen; ++i) {
        *iOut <<= 4;
        if ((iTmp = hexToInt(str[i])) < 0) {
            return -1;
        }
        *iOut |= iTmp;
    }

    return strLen;
}

/* Translate single char to int */
static int hexToInt(char c) 
{
    if ('0' <= c && c <= '9') return c - '0';
    if ('a' <= c && c <= 'f') return c - 'a' + 10;
    if ('A' <= c && c <= 'F') return c - 'A' + 10;
    return -1; // c is not a hex value
}
