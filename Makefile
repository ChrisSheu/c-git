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

RW_OBJS = src/readv_writev.o

readv_writev: $(RW_OBJS)
	$(CC) -o $@ $(RW_OBJS) $(CFLAGS)
	$(STRIP) $@
	mkdir -p output
	-mv $@ output/$@
	ls -lFh --color output/

all: clean readv_writev

clean:
	-rm -f src/*.o
	-rm -rf output/*

