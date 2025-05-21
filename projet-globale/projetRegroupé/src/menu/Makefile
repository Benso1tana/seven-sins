CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf

all: menu.o

menu.o: menu_principale.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o
