prog:main.o source.o
	gcc main.o source.o -o prog -lSDL -g -lSDL_image -lSDL_mixer -lSDL_ttf
main.o:main.c
	gcc -c main.c -g

source.o:source.c
	gcc -c source.c -g
