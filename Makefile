#vim: set ts=8 sw=8 noet:

PWD_DIR = $(pwd)
#CROSS_COMPILE = arm-linux-
CC            = $(CROSS_COMPILE)gcc
CXX           = $(CROSS_COMPILE)g++
CPLUSPLUS     = $(CROSS_COMPILE)g++
STRIP         = $(CROSS_COMPILE)strip
AR            = $(CROSS_COMPILE)ar
OBJDUMP       = $(CROSS_COMPILE)objdump


INCS = -I ./ -I include/
CFLAGS = $(INCS) -pthread

TE_OBJS = src/mytee.o src/Inet.o
CS_OBJS = src/client_sendfile.o

mytee: $(TE_OBJS)
	$(CC) -o $@ $(TE_OBJS) $(CFLAGS)
	$(STRIP) $@
	mkdir -p output
	-mv $@ output/$@
	ls -lFh --color output/

clinet_sendfile: $(CS_OBJS)
	$(CC) -o $@ $(CS_OBJS) $(CFLAGS)
	$(STRIP) $@
	mkdir -p output
	-mv $@ output/$@
	ls -lFh --color output/

clean:
	-rm -f src/*.o
	-rm -rf output/*

all: clean mytee clinet_sendfile


