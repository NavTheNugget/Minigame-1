game:
	g++ main.cpp -g -std=c++17 -Wall -o game -I include -L lib -lSDL2-2.0.0 -lSDL2_image-2.0.0 -lSDL2_ttf-2.0.0
	./game
	rm game
