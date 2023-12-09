build:
	gcc ./src/*.c `sdl2-config --libs --cflags` -lm -o bin/engine

run:
	./bin/engine

clean:
	sudo rm bin/engine
