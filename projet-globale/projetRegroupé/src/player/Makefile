CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf

all: player.o

player.o: player.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o
