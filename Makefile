#vim: set ts=8 sw=8 noet:
PROGRAM     = watch_file_descriptor
OBJECTS     = watch_file_descriptor.o

SRC_DIR     = $(shell /bin/pwd)

INCLUDES    = -I$(SRC_DIR)

#CROSS_COMPILE = 

CC          = $(CROSS_COMPILE)gcc
STRIP       = $(CROSS_COMPILE)strip

LINK        = $(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) -o $@
CFLAGS      = -g -Wall
LDFLAGS     =
LIBS        =

all:$(PROGRAM)

.SUFFIXES: .c .o

$(PROGRAM): $(OBJECTS)
	$(LINK) $(OBJECTS)
                                                                                                                                                                        
%.o: %.c
	$(CC) -c $(INCLUDES) $(CFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(PROGRAM)
