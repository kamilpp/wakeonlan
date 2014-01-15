#include <stdio.h> /* printf */
#include <stdlib.h> /* exit */
#include <string.h> /* memset & memcpy */
#include <sys/socket.h> /* socket, sendto, setsockopt */
#include <errno.h> /* errno */
#include <assert.h> /* assert */

unsigned char macAddr[7];

static int macTranslate(const char *macAddrHex, unsigned char *macAddrInt);

int main(int argc, char * const argv[])
{
    char rawMacAddr[] = "b8:27:eb:c1:71:c1";
    macTranslate(rawMacAddr, macAddr);

    return 0;
}

static int strHexToInt(const char *str, size_t strLen, unsigned *iOut);

/*  Translate MAC adress from standard format 
    XX:XX:XX:XX:XX:XX or XXXXXXXXXXXX
    to string of integers IIIIII */
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

/* Translate single char to int */
static int hexToInt(char c) 
{
    if ('0' <= c && c <= '9') return c - '0';
    if ('a' <= c && c <= 'f') return c - 'a' + 10;
    if ('A' <= c && c <= 'F') return c - 'A' + 10;
    return -1; // c is not a hex value
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
