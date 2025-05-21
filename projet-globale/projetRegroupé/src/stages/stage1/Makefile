CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf

all: stage1.o

stage1.o: source.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o
