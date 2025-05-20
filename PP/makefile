CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -lm

all: game

game: game.c
	$(CC) $(CFLAGS) -o game game.c $(LDFLAGS)

clean:
	rm -f game

.PHONY: all clean