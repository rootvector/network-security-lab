/*
 * main.c
 * RootScan - Entry Point
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rootscan.h"
#include "db.h"

static void print_usage(const char *prog) {
    printf("Usage:\n");
    printf("  %s <target_ip> <start_port> <end_port>\n\n", prog);
    printf("Example:\n");
    printf("  %s 127.0.0.1 1 1000\n", prog);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        print_usage(argv[0]);
        return 1;
    }

    /* -------- Parse arguments -------- */
    scan_result_t result;
    memset(&result, 0, sizeof(result));

    strncpy(result.target, argv[1], sizeof(result.target) - 1);
    result.start_port = atoi(argv[2]);
    result.end_port   = atoi(argv[3]);

    if (result.start_port <= 0 || result.end_port > 65535 ||
        result.start_port > result.end_port) {
        fprintf(stderr, "Invalid port range\n");
        return 1;
    }

    /* -------- Init database -------- */
    if (db_init() != 0) {
        fprintf(stderr, "Failed to connect to database\n");
        return 1;
    }

    printf("[*] Starting TCP scan on %s (%d-%d)\n",
           result.target, result.start_port, result.end_port);

    /* -------- Run TCP scan -------- */
    if (tcp_scan(&result) != 0) {
        fprintf(stderr, "Scan failed\n");
        db_close();
        return 1;
    }

    printf("[+] Scan finished\n");
    printf("[+] Open ports found: %d\n", result.open_port_count);
    printf("[+] Log file: %s\n", result.stdout_file);

    /* -------- Insert into DB -------- */
    if (db_insert_scan(&result) != 0) {
        fprintf(stderr, "Failed to insert scan result into database\n");
        db_close();
        return 1;
    }

    printf("[+] Scan result saved to database\n");

    /* -------- Cleanup -------- */
    db_close();
    return 0;
}

