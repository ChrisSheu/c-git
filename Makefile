#vim: set ts=8 sw=8 noet:
    
PWD_DIR = $(pwd)
CROSS_COMPILE = arm-hisiv100nptl-linux-
CC            = $(CROSS_COMPILE)gcc
CXX           = $(CROSS_COMPILE)g++
CPLUSPLUS     = $(CROSS_COMPILE)g++
STRIP         = $(CROSS_COMPILE)strip
AR            = $(CROSS_COMPILE)ar
OBJDUMP       = $(CROSS_COMPILE)objdump


INCS = -I ./ -I include/
CFLAGS = $(INCS)

FILENAME=fork

OBJS = src/$(FILENAME).c

$(FILENAME):
	$(CC) -o $@ $(OBJS) $(CFLAGS)
	$(STRIP) $@
	mkdir -p output
	-mv $@ output/$@
	ls -lFh --color output/

all: $(FILENAME)

clean:
	-rm -rf src/*.o
	-rm -rf output/*
