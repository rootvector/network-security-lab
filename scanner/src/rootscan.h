#ifndef ROOTSCAN_H
#define ROOTSCAN_H

#include <time.h>

#define MAX_OPEN_PORTS 1024
#define MAX_PATH_LEN 256
#define MAX_IP_LEN 64

/* SCAN TYPES */
typedef enum{
    SCAN_TCP,
    SCAN_UDP,
    SCAN_SYN,
    SCAN_NULL
} scan_type_t;

/* SCAN STATUS  */
typedef enum {
    SCAN_SUCCESS,
    SCAN_FAILED
} scan_status_t;

/*  SCAN RESULT STRUCTURE */
typedef struct{
    scan_type_t scan_type;
    char target[MAX_IP_LEN];
    int start_port;
    int end_port;

    int open_ports[MAX_OPEN_PORTS];
    int open_port_count;

    long duration_ms;
    scan_status_t status;

    char stdout_file[MAX_PATH_LEN];
    
    time_t scan_time;
} scan_result_t;

/*  SCANNER FUNCTIONS   */
int tcp_scan(scan_result_t *result);
int udp_scan(scan_result_t *result);
int syn_scan(scan_result_t *result);
int null_scan(scan_result_t *result);




//int rootscan(const char *ip, int port);
void fatalerr(const char *err);
#endif
