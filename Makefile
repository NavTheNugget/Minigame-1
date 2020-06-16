game:
	g++ main.cpp -std=c++11 -Wall -o game -I include -L lib -lSDL2-2.0.0
	./game
	rm game
