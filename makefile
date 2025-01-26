

jooj.exe: main.o
	gcc main.o -o jooj.exe  -O3 -Wall -std=c99 -Wno-missing-braces -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm
main.o: main.c particle.h vec_utils.h
	gcc -c main.c -O3 -Wall -std=c99 -Wno-missing-braces -I include/
