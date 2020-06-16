#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <vector>
#include <map>
#include <random>
using namespace std;

typedef struct {
	float x, y;
} Vector2f;
typedef struct {
	Vector2f location;
	bool destroyed = false;
} Object;

vector<Object> objects;
random_device rd;
default_random_engine rng(rd());
const unsigned int SCREEN_WIDTH = 640;
const unsigned int SCREEN_HEIGHT = 480;
const std::string SCREEN_TITLE = "Minigame";
const Uint8* keyboardState = SDL_GetKeyboardState(0);

void generateGame(unsigned int maxObjects);
bool isKeyPressed(SDL_Scancode key);
int getRandomInt(int min, int max);
int main(int argc, char* argv[]) {
	
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow(SCREEN_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	bool running = true;
	SDL_Event event;
	
	generateGame(10);
	
	Vector2f playerVelocity;
	SDL_Rect player = {10,10,25,25};
	playerVelocity.x=playerVelocity.y=0;
	const float playerSpeed = 2.5f;
	
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;
	
	Uint32 frameStart, frameTime;
	
	while (running) {
		frameStart = SDL_GetTicks();
		
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT)
			running = false;
		
		if (isKeyPressed(SDL_SCANCODE_A))
			playerVelocity.x = -playerSpeed;
		else if (isKeyPressed(SDL_SCANCODE_D))
			playerVelocity.x = playerSpeed;
		
		player.x += playerVelocity.x;
		
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(renderer, &player);
		
		for (unsigned int i = 0; i < objects.size(); ++i) {
			SDL_Rect rect = { (int)objects[i].location.x, (int)objects[i].location.y, 10, 10 };
			SDL_RenderFillRect(renderer, &rect);
		}
		
		SDL_RenderPresent(renderer);
		
		playerVelocity.x = playerVelocity.y = 0;
		
		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime)
			SDL_Delay(frameDelay-frameTime);
	}
	
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	
	return 0;
}

bool isKeyPressed(SDL_Scancode key) {
	if (keyboardState)
		if (keyboardState[key])
			return true;
	return false;
}
void generateGame(unsigned int maxObjects) {
	Vector2f previous;
	for (unsigned int i = 0; i < maxObjects; ++i) {
		Object o;
		Vector2f location;
		if (i == 0)
			location.x = getRandomInt(50, SCREEN_WIDTH-50);
		else
			if (previous.x > SCREEN_WIDTH-50/2)
				location.x = getRandomInt(50,SCREEN_WIDTH/2);
			else
				location.x = getRandomInt(SCREEN_WIDTH/2+50,SCREEN_WIDTH-50);
			
		if (i == 0)
			 location.x = getRandomInt(50, SCREEN_WIDTH-50);
		else 
			if (previous.y > SCREEN_HEIGHT-50/2)
				location.y = getRandomInt(50, SCREEN_HEIGHT/2);
			else 
				location.y = getRandomInt(SCREEN_HEIGHT/2+50,SCREEN_HEIGHT);
				
		o.location = location;
		
		objects.push_back(o);
		
		previous = location;
	}
}
int getRandomInt(int min, int max) {
	uniform_int_distribution<int> dist(min, max);
	return dist(rng);
}