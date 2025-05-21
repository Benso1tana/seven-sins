prog: main.o source.o
	gcc main.o source.o -o prog -lSDL -g -lSDL_ttf -lSDL_image

main.o: main.c header.h
	gcc -c main.c -g

source.o: source.c header.h
	gcc -c source.c -g
