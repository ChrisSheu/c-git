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

MMAP_R_OBJS = src/mmap_read.o
MMAP_W_OBJS = src/mmap_write.o

mmap_read: $(MMAP_R_OBJS)
	$(CC) -o $@ $(MMAP_R_OBJS) $(CFLAGS)
	$(STRIP) $@
	mkdir -p output
	-mv $@ output/$@
	ls -lFh --color output/

mmap_write: $(MMAP_W_OBJS)
	$(CC) -o $@ $(MMAP_W_OBJS) $(CFLAGS)
	$(STRIP) $@
	mkdir -p output
	-mv $@ output/$@
	ls -lFh --color output/

clean:
	-rm -f src/*.o
	-rm -rf output/*

all: clean mmap_read mmap_write


