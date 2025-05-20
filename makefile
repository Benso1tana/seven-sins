prog: main.o source.o
	gcc main.o source.o -o prog `sdl-config --libs` -lSDL_image -lSDL_ttf -lm -g 

main.o: main.c header.h
	gcc -c main.c -g

source.o: source.c header.h
	gcc -c source.c -g
