RM=/bin/rm
CC=/usr/bin/gcc
CFLAGS=-g -Wall -O3
MAKE=/usr/bin/make

all: position_logger

position_logger:
	${CC} ${CFLAGS} -o position_logger position_logger.c -lgps -lm

.PHONY: clean

clean:
	rm -rf position_logger

