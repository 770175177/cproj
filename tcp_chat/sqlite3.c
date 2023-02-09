#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

static sqlite3 *db = NULL;

int sql_init(void);
void sql_close(void);
int sql_show(const char *name);
int sql_delete(const char *name);
int sql_insert(const char *name, const char *passwd);
int sql_update(const char *name, const char *passwd);
static int show_callback(void *data, int argc, char **argv, char **colName);
#if 0
int main(int argc, const char *argv[])
{
	int ret = 0;

	ret = sql_init();
	if (ret < 0)
	{
		return -1;
	}

	/*
	ret = sql_insert("liang", "123");
	if (ret < 0)
	{
		return -1;
	}
	*/
#if 0
	ret = sql_show("liang");
	if (ret < 0)
	{
		return -1;
	}
#endif
#if 0
	ret = sql_delete("liang");
	if (ret < 0)
	{
		return -1;
	}
#endif
	ret = sql_update("liang", "asdf123");
	if (ret < 0)
	{
		return -1;
	}

	sql_close();
}
#endif

int sql_init(void)
{
	int ret = 0;

	ret = sqlite3_open("phone.db", &db);
	if (SQLITE_OK != ret)
	{
		printf("sqlite_opne fail, reason: %s\n", sqlite3_errmsg(db));
		return -1;
	}
	if (NULL == db)
	{
		sqlite3_close(db);
		return -1;
	}

	return 0;
}

void sql_close(void)
{
	if (NULL != db)
	{
		sqlite3_close(db);
	}
}

int sql_insert(const char *name, const char *passwd)
{
	char cmd_buf[128] = {0};
	char *err = NULL;
	int ret = 0;

	if ((NULL == name) || (passwd == NULL) || (db == NULL))
	{
		printf("name is NULL or passed is NULL or db is NULL\n");
		return -1;
	}

	memset(cmd_buf, 0, sizeof(cmd_buf));
	sprintf(cmd_buf, "insert into user values('%s', '%s');", name, passwd);

	ret = sqlite3_exec(db, cmd_buf, NULL, NULL, &err);
	if (SQLITE_OK != ret)
	{
		printf("sql_insert fail, reason: %s\n", err);
		return -1;
	}

	return 0;
}

int sql_delete(const char *name)
{
	char cmd_buf[128] = {0};
	char *err = NULL;
	int ret = 0;

	if ((NULL == name) && (db == NULL))
	{
		printf("name is NULL or db is NULL\n");
		return -1;
	}

	memset(cmd_buf, 0, sizeof(cmd_buf));
	sprintf(cmd_buf, "delete from user where name in ('%s');", name);

	ret = sqlite3_exec(db, cmd_buf, NULL, NULL, &err);
	if (SQLITE_OK != ret)
	{
		printf("sql_delet fail, reason: %s\n", err);
		return -1;
	}

	return 0;
}

int sql_update(const char *name, const char *passwd)
{
	char cmd_buf[128] = {0};
	char *err = NULL;
	int ret = 0;

	if ((db == NULL) || (name == NULL) || (passwd == NULL))
	{
		printf("db is NULL or name is NULL or passwd is NULL\n");
		return -1;
	}

	memset(cmd_buf, 0, sizeof(cmd_buf));
	sprintf(cmd_buf, "update user set passwd = '%s' where name = '%s';", passwd, name);

	ret = sqlite3_exec(db, cmd_buf, NULL, NULL, &err);
	if (SQLITE_OK != ret)
	{
		printf("sql_update fail, reason: %s\n", err);
		return -1;
	}

	return 0;
}

int sql_show(const char *name)
{
	char cmd_buf[128] = {0};
	char *err = NULL;
	int ret = 0;

	if (db == NULL)
	{
		printf("db is NULL\n");
		return -1;
	}

	memset(cmd_buf, 0, sizeof(cmd_buf));
	sprintf(cmd_buf, "select * from user;");

	ret = sqlite3_exec(db, cmd_buf, show_callback, (void *)name, &err);
	if (SQLITE_OK != ret)
	{
		printf("sql_show fail, reason: %s\n", err);
		return -1;
	}

	return 0;
}

static int show_callback(void *data, int argc, char **argv, char **colName)
{
	int i = 0;
	const char *name = (const char *)data;

	if ((NULL != name) && (strcmp(name, argv[0])))
	{
		return 0;
	}

	for (i = 0; i < argc; i++)
	{
		printf("%s = %s\n", colName[i], argv[i] ? argv[i] : "NULL");
	}

	printf("\n");

	return 0;
}
