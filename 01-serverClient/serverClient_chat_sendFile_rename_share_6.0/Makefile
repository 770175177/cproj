CROSS=arm-linux-gnueabi-gcc

s : server.c 
	$(CC) server.c -lpthread -o s.o

c : client.c 
	$(CC) client.c -lpthread -o c.o

sql : sqlie3.c 
	$(CC) sqlite3.c -lsqlite3 -o sql.o

clean:
	rm *.o
