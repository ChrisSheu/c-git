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
CFLAGS = $(INCS)

SF_OBJS = src/client_sendfile.o
SERV_OBJS = src/server_splice.o src/Inet.o

client_sendfile: $(SF_OBJS)
	$(CC) -o $@ $(SF_OBJS) $(CFLAGS)
	$(STRIP) $@
	mkdir -p output
	-mv $@ output/$@
	ls -lFh --color output/

server_splice: $(SERV_OBJS)
	$(CC) -o $@ $(SERV_OBJS) $(CFLAGS)
	$(STRIP) $@
	mkdir -p output
	-mv $@ output/$@
	ls -lFh --color output/

all: clean client_sendfile server_splice

clean:
	-rm -f src/*.o
	-rm -rf output/*

