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

SHM_READ_OBJS = src/shm_read.o

shm_read: $(SHM_READ_OBJS)
	$(CC) -o $@ $(SHM_READ_OBJS) $(CFLAGS)
	$(STRIP) $@
	mkdir -p output
	-mv $@ output/$@
	ls -lFh --color output/

SHM_WRITE_OBJS = src/shm_write.o

shm_write: $(SHM_WRITE_OBJS)
	$(CC) -o $@ $(SHM_WRITE_OBJS) $(CFLAGS)
	$(STRIP) $@
	mkdir -p output
	-mv $@ output/$@
	ls -lFh --color output/

all: shm_read shm_write

clean:
	-rm -f src/*.o 
	-rm -rf output/*

