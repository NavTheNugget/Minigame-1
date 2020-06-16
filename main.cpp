#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <vector>
#include <map>
using namespace std;

int main(int argc, char* argv[]) {
	const unsigned int SCREEN_WIDTH = 640;
	const unsigned int SCREEN_HEIGHT = 480;
	const std::string SCREEN_TITLE = "Minigame";
	
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow(SCREEN_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	bool running = true;
	SDL_Event event;
	
	while (running) {
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT)
			running = false;
	}
	
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	
	return 0;
}