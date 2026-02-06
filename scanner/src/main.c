#include <stdio.h>
#include <stdlib.h>
#include "rootscan.h"

int usage(const char *filename) {
    printf("USAGE: %s <IPv4> <start_port> <end_port>\n", filename);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        usage(argv[0]);
        return -1;
    }

    const char *target_ip = argv[1];
    int start_port = atoi(argv[2]);
    int end_port   = atoi(argv[3]);

    if (start_port <= 0 || end_port > 65535 || start_port >= end_port) {
        printf("Invalid port range\n");
        return -1;
    }

    printf("Scanning %s from %d to %d...\n",
           target_ip, start_port, end_port);

    for (int port = start_port; port <= end_port; port++) {
        if (rootscan(target_ip, port) == 0) {
            printf("[OPEN] Port %d\n", port);
        }
    }

    return 0;
}

