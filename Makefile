game: src/dungeon.h src/*.c
	mkdir -p bin
	cc -o bin/game.bin src/*.c -O3 -Wall -Werror -Wextra -l5kgl -lglfw -lGL -lGLEW -lX11 -lXrandr -lXi -lXxf86vm -lpthread -lm
