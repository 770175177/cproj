CROSS=arm-linux-gnueabi-gcc
CC=gcc


share : share.c share.h
	$(CC) share.c -lpthread -o share
	$(CROSS) share.c -lpthread -o arm-share

clean:
	rm share arm-share
