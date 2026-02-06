#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    MYSQL *conn;

    conn = mysql_init(NULL);
    if (!conn) {
        printf("mysql_init failed\n");
        return 1;
    }

    if (!mysql_real_connect(
            conn,
            "localhost",     // host
            "admin",       // user
            "admin",      // password
            "scanner_db",    // database
            0, NULL, 0)) {

        printf("Connection failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        return 1;
    }

    const char *query =
        "INSERT INTO Scans (target, port, status) "
        "VALUES ('127.0.0.1', 22, 'OPEN')";

    if (mysql_query(conn, query)) {
        printf("Insert failed: %s\n", mysql_error(conn));
    } else {
        printf("Insert successful\n");
    }

    mysql_close(conn);
    return 0;
}

