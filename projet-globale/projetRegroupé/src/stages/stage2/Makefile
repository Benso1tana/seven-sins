CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf

all: stage2.o

stage2.o: source.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o
