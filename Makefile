all:
	gcc main.c engine.c -o prog -I "include" -L "lib" -lSDL2main -lSDL2 -o ./build/doom_fire.out; ./build/doom_fire.out