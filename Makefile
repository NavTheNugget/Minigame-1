run:
	g++ main.cpp -g -std=c++17 -Wall -o game `pkg-config --libs --cflags sdl2 sdl2_ttf sdl2_image`
	./game
