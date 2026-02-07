/*
 * tcp_scan.c
 * RootScan - TCP Connect Scan Implementation
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <time.h>

#include "rootscan.h"

/* Get current time in milliseconds */
static long get_time_ms(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000L) + (tv.tv_usec / 1000L);
}

/* Ensure logs directory exists */
static void create_logs_dir(void) {
    struct stat st;
    if (stat("logs", &st) == -1) {
        mkdir("logs", 0755);
    }
}

/* TCP connect() scan */
int tcp_scan(scan_result_t *result) {
    if (!result)
        return -1;

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;

    /* Convert target IP */
    if (inet_pton(AF_INET, result->target, &addr.sin_addr) != 1) {
        result->status = SCAN_FAILED;
        return -1;
    }   

    /* Prepare logging */
    create_logs_dir();

    snprintf(result->stdout_file, MAX_PATH_LEN,
             "logs/scan_%s_tcp.txt", result->target);

    FILE *log = fopen(result->stdout_file, "w");
    if (!log) {
        result->status = SCAN_FAILED;
        return -1;
    }

    /* Log header */
    fprintf(log, "RootScan TCP Scan\n");
    fprintf(log, "Target: %s\n", result->target);
    fprintf(log, "Port Range: %d-%d\n\n",
            result->start_port, result->end_port);

    /* Start scan */
    long start_ms = get_time_ms();
    result->open_port_count = 0;

    for (int port = result->start_port;
         port <= result->end_port;
         port++) {

        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0)
            continue;

        addr.sin_port = htons(port);

        if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == 0) {

            if (result->open_port_count < MAX_OPEN_PORTS) {
                result->open_ports[result->open_port_count++] = port;
            }

            fprintf(log, "[OPEN] %d\n", port);
        }

        close(sock);
    }

    /* Finish scan */
    long end_ms = get_time_ms();

    result->duration_ms = end_ms - start_ms;
    result->scan_type   = SCAN_TCP;
    result->status      = SCAN_SUCCESS;
    result->scan_time   = time(NULL);

    /* Log footer */
    fprintf(log, "\nScan completed\n");
    fprintf(log, "Open ports: %d\n", result->open_port_count);
    fprintf(log, "Duration: %ld ms\n", result->duration_ms);

    fclose(log);
    return 0;
}

