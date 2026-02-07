#ifndef DB_H
#define DB_H

#include "rootscan.h"

/* Initialize database connection */
int db_init(void);

/* Insert scan result into database */
int db_insert_scan(const scan_result_t *result);

/* Close database connection */
void db_close(void);

#endif /* DB_H */

