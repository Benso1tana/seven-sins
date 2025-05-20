# stage .1/Makefile
CC = gcc
CFLAGS = -Wall -g -I. -I..
LDFLAGS = -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf

SRCS = source.c
OBJS = $(SRCS:.c=.o)

all: $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

.PHONY: all clean