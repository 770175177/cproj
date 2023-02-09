CC = gcc

all : server.c client.c sqlite3.c
	$(CC) server.c sqlite3.c -lsqlite3 -lpthread -std=gnu99 -o s
	$(CC) client.c -lpthread -std=gnu99 -o c
	#$(CC) sqlite3.c -lsqlite3 -std=gnu99 -o sql

clean :
	rm s c sql
