prog: main.o p.o 
	gcc main.o p.o -o prog -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf

main.o: main.c
	gcc -c main.c -g
p.o:p.c
	gcc -c p.c -g
clean:
	rm -f prog *.o

