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

DEADLOCK_OBJS = src/deadlock.o
FORK_OBJS = src/fork.o
ATFORK_OBJS = src/fork_atfork.o
SIG_OBJS = src/sig.o

deadlock: $(DEADLOCK_OBJS)
	$(CC) -o $@ $(DEADLOCK_OBJS) $(CFLAGS)
	$(STRIP) $@
	mkdir -p output
	-mv $@ output/$@
	ls -lFh --color output/

fork: $(FORK_OBJS)
	$(CC) -o $@ $(FORK_OBJS) $(CFLAGS)
	$(STRIP) $@
	mkdir -p output
	-mv $@ output/$@
	ls -lFh --color output/

atfork: $(ATFORK_OBJS)
	$(CC) -o $@ $(ATFORK_OBJS) $(CFLAGS)
	$(STRIP) $@
	mkdir -p output
	-mv $@ output/$@
	ls -lFh --color output/

sig: $(SIG_OBJS)
	$(CC) -o $@ $(SIG_OBJS) $(CFLAGS)
	$(STRIP) $@
	mkdir -p output
	-mv $@ output/$@
	ls -lFh --color output/

all: deadlock fork atfork sig

clean:
	-rm -f src/*.o
	-rm -rf output/*

