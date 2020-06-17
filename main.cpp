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

random_device rd;
default_random_engine rng(rd());
bool init = false;
const unsigned int SCREEN_WIDTH = 640;
const unsigned int SCREEN_HEIGHT = 480;
const std::string SCREEN_TITLE = "Minigame";
const Uint8* keyboardState = SDL_GetKeyboardState(0);

bool isKeyPressed(SDL_Scancode key);
int getRandomInt(int min, int max);
Object* generateObject(bool init, Vector2f previousLocation);

int main(int argc, char* argv[]) {
	Vector2f previousLocation; previousLocation.x = previousLocation.y = 0;
	
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow(SCREEN_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	bool running = true;
	SDL_Event event;
	
	Vector2f playerVelocity;
	SDL_Rect player = {10,10,25,25};
	playerVelocity.x=playerVelocity.y=0;
	const float playerSpeed = 2.5f;
	
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;
	
	Uint32 frameStart, frameTime;
	
	Object* o = generateObject(init, previousLocation);
	init = true;
	
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
		
		if (!o->destroyed) {
			SDL_Rect objectRect = { (int)o->location.x, (int)o->location.y, 15, 15 };
			SDL_RenderFillRect(renderer, &objectRect);
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
	
	delete o;
	
	return 0;
}

bool isKeyPressed(SDL_Scancode key) {
	if (keyboardState)
		if (keyboardState[key])
			return true;
	return false;
}
Object* generateObject(bool init, Vector2f previousLocation) {
	Object* object = new Object();
	if (!init) {
		object->location.x = getRandomInt(0, SCREEN_WIDTH);
		object->location.y = getRandomInt(0, SCREEN_HEIGHT);
	} else {
		if (previousLocation.x > SCREEN_WIDTH/2) {
			object->location.x = getRandomInt(0, SCREEN_WIDTH/2);
		} else {
			object->location.x = getRandomInt(SCREEN_WIDTH/2, SCREEN_WIDTH);
		}
		
		if (previousLocation.y > SCREEN_HEIGHT/2) {
			object->location.y = getRandomInt(0, SCREEN_HEIGHT);
		} else {
			object->location.y = getRandomInt(SCREEN_HEIGHT/2, SCREEN_HEIGHT);
		}
	}
	return object;
}
int getRandomInt(int min, int max) {
	uniform_int_distribution<int> dist(min, max);
	return dist(rng);
}