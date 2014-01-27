#include <stdio.h> /* printf */
#include <stdlib.h> /* exit */
#include <string.h> /* memset & memcpy */
#include <sys/socket.h> /* socket, sendto, setsockopt */
#include <arpa/inet.h> /* htonl & htons */
#include <netdb.h> /* gethostbyname */
#include <errno.h> /* errno */
#include <assert.h> /* assert */
#include <unistd.h> /* close, getopt, fork, execlp, dup2 */
#include <sys/select.h> /* select, fd_set */
#include <sys/wait.h>
#define QUEUE_SIZE 5
#define PORT 1500

typedef enum {
    false,
    true 
} bool;

int listenTCP(int port);
int getConnection(int sock);

int sock, sockClient;

int main(int argc, char *argv[]) 
{
    if ((sock = listenTCP(PORT)) < 0) {
        printf("Server could not be started!\n");
        return -1;
    }

    while (true) {
        if ((sockClient = getConnection(sock)) <= 0) {
            if (sockClient < 0) {
                printf("Could not get new connections!\n");
                return -1;
            }
        } else if (!fork()) {
            close(sock);
            char buff[100];
            size_t nRead;

            while (true) {
                memset(buff, 0, sizeof(buff));
                if ((nRead = recv(sockClient, &buff, sizeof(buff), 0)) <= 0) {
                    if (nRead < 0) {
                        perror("ERROR recv");
                        return -1;
                    } else {
                        break;
                    }
                } else {
                    if (!strncmp(buff, "shutdown", 8)) {
                        printf("Shutting down the system...\n");
                        // dup2(sockClient, 1);
                        // dup2(sockClient, 2);
                        execlp("shutdown", "shutdown", "-h", "now", NULL);
                        break;
                    } else if (!strncmp(buff, "test", 4)) {
                        printf("Test...\n");
                        int pid;
                        if (!(pid = fork())) {
                            dup2(sockClient, 1);
                            dup2(sockClient, 2);
                            close(sockClient);
                            execlp("ls", "ls", NULL);
                        }
                        waitpid(pid, NULL, 0);
                    } else {
                        printf("Unknown request: %s", buff);
                    }
                }
            }
            printf("Terminating connection...\n");
            close(sockClient);
            return 0;
        }
    }

    close(sock);
    return 0;
}

int listenTCP(int port) 
{
    printf("Starting TCP server on port %d...\n", port);
    if (port < 1024) {
        printf("Warning! Port %d in superuser port range...", port);
    }

    int sock;
    int optval = 1;
    struct hostent* host;
    struct sockaddr_in sAddr;
    size_t sAddrSize = sizeof(sAddr);

    /* Get ip adress */     
    host = gethostbyname("localhost");
    if (!host) {
        perror("gethostbyname");
        return -1;
    }

    /* Create a TCP socket */
    if ((sock = socket(host->h_addrtype, SOCK_STREAM, 0)) <= 0) {
        perror("socket");
        return -1;
    }
    
    /* Allow immediate reusing of socket port */
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval)) < 0) {
        perror("setsockopt");
        close(sock);
        return -1;
    }

    /* Preapeare address structure */
    sAddr.sin_family = host->h_addrtype;
    sAddr.sin_port = htons(port);
    memcpy(&sAddr.sin_addr.s_addr, host->h_addr_list[0], host->h_length);

    /* Bind name to socket */
    if (bind(sock, (struct sockaddr*)&sAddr, sAddrSize) < 0) {     
        perror("bind");
        close(sock);
        return -1;
    }

    /* Run and specify queue size */
    if (listen(sock, QUEUE_SIZE) < 0) {
        perror("listen");
        close(sock);
        return -1;
    }

    printf("Server listening on port %d\n", port);
    return sock;
}

int getConnection(int sock) 
{
    static struct sockaddr_in sAddrClient;
    static struct timeval tv;
    static int sockClient, nFound;
    static size_t sAddrSize = sizeof(sAddrClient);
    static fd_set fdRmask;

    /* Watch main socket descriptor */
    FD_ZERO(&fdRmask);
    FD_SET(sock, &fdRmask);

    /* Set timeout to one second */
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    if ((nFound = select(sock + 1, &fdRmask, NULL, (fd_set*)0, &tv)) <= 0) {
        if (nFound < 0) {
            perror("ERROR select");
            return -1;
        }
        return 0;
    }

    /* New connections awaiting */
    if (FD_ISSET(sock, &fdRmask)) {
        if ((sockClient = accept(sock, (struct sockaddr*)&sAddrClient, (socklen_t *)&sAddrSize)) < 0) {
            perror("ERROR accept");
            return -1;
        }
        printf("[connection from %s]\n", inet_ntoa((struct in_addr)sAddrClient.sin_addr));
    }

    return sockClient;
}