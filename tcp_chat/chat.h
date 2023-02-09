#ifndef __CHAT_H__
#define __HCAT_H__

/* sqlite3 */
int sql_init(void);
void sql_close(void);
int sql_show(const char *name);
int sql_delete(const char *name);
int sql_insert(const char *name, const char *passwd);
int sql_update(const char *name, const char *passwd);
static int show_callback(void *data, int argc, char **argv, char **colName);

/* server */
#define SERVER_IP_ADDR    "127.0.0.1"
#define SERVER_IP_PORT    8000

#endif
