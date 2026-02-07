/*
 * db.c
 * RootScan - MySQL Database Interface
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#include "db.h"

/* -------- CONFIG -------- */
#define DB_HOST "localhost"
#define DB_USER "admin"
#define DB_PASS "admin"
#define DB_NAME "rootscans"
#define DB_PORT 3306
/* ------------------------ */

static MYSQL *conn = NULL;

/* Convert scan_type enum to string */
static const char *scan_type_to_string(scan_type_t type) {
    switch (type) {
        case SCAN_TCP:  return "TCP";
        case SCAN_UDP:  return "UDP";
        case SCAN_SYN:  return "SYN";
        case SCAN_NULL: return "NULL";
        default:        return "TCP";
    }
}

/* Convert open_ports[] to comma-separated string */
static void open_ports_to_string(
    const scan_result_t *result,
    char *buffer,
    size_t size
) {
    buffer[0] = '\0';

    for (int i = 0; i < result->open_port_count; i++) {
        char temp[16];
        snprintf(temp, sizeof(temp), "%d", result->open_ports[i]);
        strncat(buffer, temp, size - strlen(buffer) - 1);

        if (i < result->open_port_count - 1)
            strncat(buffer, ",", size - strlen(buffer) - 1);
    }
}

/* Initialize MySQL connection */
int db_init(void) {
    conn = mysql_init(NULL);
    if (!conn) {
        fprintf(stderr, "mysql_init failed\n");
        return -1;
    }

    if (!mysql_real_connect(
            conn,
            DB_HOST,
            DB_USER,
            DB_PASS,
            DB_NAME,
            DB_PORT,
            NULL,
            0)) {

        fprintf(stderr, "MySQL error: %s\n", mysql_error(conn));
        return -1;
    }

    return 0;
}

/* Insert scan result */
int db_insert_scan(const scan_result_t *result) {
    if (!conn || !result)
        return -1;

    char ports[2048];
    open_ports_to_string(result, ports, sizeof(ports));

    char query[4096];
    snprintf(query, sizeof(query),
        "INSERT INTO scans "
        "(scan_type, target, start_port, end_port, open_ports, "
        "duration_ms, status, stdout_file) "
        "VALUES ('%s','%s',%d,%d,'%s',%ld,'%s','%s')",
        scan_type_to_string(result->scan_type),
        result->target,
        result->start_port,
        result->end_port,
        ports,
        result->duration_ms,
        (result->status == SCAN_SUCCESS ? "SUCCESS" : "FAILED"),
        result->stdout_file
    );

    if (mysql_query(conn, query)) {
        fprintf(stderr, "MySQL insert error: %s\n", mysql_error(conn));
        return -1;
    }

    return 0;
}

/* Close DB connection */
void db_close(void) {
    if (conn) {
        mysql_close(conn);
        conn = NULL;
    }
}

