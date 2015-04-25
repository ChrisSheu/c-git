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
CFLAGS_LOCK = $(INCS) -DFCNTL_LOCK

DUP_OBJS = src/dup_dup2.c

dup_dup2: $(DUP_OBJS)
	$(CC) -o $@ $(DUP_OBJS) $(CFLAGS)
	$(STRIP) $@
	mkdir -p output
	-mv $@ output/$@
	ls -lFh --color output/

dup_dup2_fcntl: $(DUP_OBJS)
	$(CC) -o $@ $(CFLAGS_LOCK) $(DUP_OBJS)
	$(STRIP) $@
	mkdir -p output
	-mv $@ output/$@
	ls -lFh --color output/

clean:
	-rm -f src/*.o
	-rm -rf output/*

all: clean dup_dup2 dup_dup2_fcntl


