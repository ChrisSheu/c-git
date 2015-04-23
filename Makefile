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

PIPE_OBJS = src/pipe.o

pipe: $(PIPE_OBJS)
	$(CC) -o $@ $(PIPE_OBJS) $(CFLAGS)
	$(STRIP) $@
	mkdir -p output
	-mv $@ output/$@
	ls -lFh --color output/

all: pipe

clean:
	-rm -f src/*.o
	-rm -rf output/*

