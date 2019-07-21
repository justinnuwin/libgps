RM=/bin/rm
CC=/usr/bin/gcc
CFLAGS=-g -Wall -O3
MAKE=/usr/bin/make
LIBS=-lgps -lm

all: position_logger

position_logger: position_logger.c
	${CC} ${CFLAGS} -o $@ $^ $(LIBS)

.PHONY: clean

clean:
	rm -rf position_logger

