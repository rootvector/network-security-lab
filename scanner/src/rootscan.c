#include "rootscan.h"
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int rootscan(const char *ip, int port) {
    int sock;
    struct sockaddr_in target;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        return -1;

    target.sin_family = AF_INET;
    target.sin_port = htons(port);

    if (inet_pton(AF_INET, ip, &target.sin_addr) != 1) {
        close(sock);
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&target, sizeof(target)) == 0) {
        close(sock);
        return 0;   // OPEN
    }

    close(sock);
    return -1;      // CLOSED
}


void fatalerr(const char *err) {
    fprintf(stderr, "[!] %s\n", err);
}

