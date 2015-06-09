#vim: set ts=8 sw=8 noet:
PROGRAM     = select
OBJECTS     = src/select.o

SRC_DIR       = $(shell /bin/pwd)
TFTP_DIR      = /tftpboot
OUTPUT_DIR    = output/

INCLUDES    = -I$(SRC_DIR)

#CROSS_COMPILE = arm-hisiv100-linux-
CC          = $(CROSS_COMPILE)gcc
STRIP       = $(CROSS_COMPILE)strip

CFLAGS      = -g -Wall
LDFLAGS     =
LIBS        =
LINK        = $(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) -o $(OUTPUT_DIR)$@

all:$(PROGRAM)

.SUFFIXES: .c .o

$(PROGRAM): $(OBJECTS)
	$(LINK) $(OBJECTS)

%.o: %.c
	$(CC) -c $(INCLUDES) $(CFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(OUTPUT_DIR)$(PROGRAM)
