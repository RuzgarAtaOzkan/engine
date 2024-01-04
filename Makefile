build:
	gcc -Wall -std=c11 ./src/*.c `sdl2-config --libs --cflags` -lm -o bin/3drenderer

run:
	./bin/3drenderer

clean:
	sudo rm bin/3drenderer
