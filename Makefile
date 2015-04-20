#vim: set ts=8 sw=8 noet:

CROSS-COMPILE=
CC=gcc
CFLAGS=

obj=src/ntpdate.c

TARGET=ntpclient

$(TARGET):
	$(CROSS-COMPILE)$(CC) -o $@ $(obj)

clean:
	rm -rf src/*.o
	rm -rf $(TARGET)
