#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
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
typedef struct {
	unsigned long long int score;
} Game;

random_device rd;
default_random_engine rng(rd());
bool init = false;
const unsigned int SCREEN_WIDTH = 640;
const unsigned int SCREEN_HEIGHT = 480;
const std::string SCREEN_TITLE = "Colliders";
const Uint8* keyboardState = SDL_GetKeyboardState(0);

int currentScreen = 0; // -1 Menu 0 Game 1 Credits 2 Controls
int timer = 0;

bool isKeyPressed(SDL_Scancode key);
int getRandomInt(int min, int max);
double getRandomDouble(double min, double max);
Object* generateObject(bool init, Vector2f previousLocation);

bool checkCollision(Object* object, SDL_Rect player);

int main(int argc, char* argv[]) {
	Vector2f previousLocation; previousLocation.x = previousLocation.y = 0;
	
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow(SCREEN_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	bool running = true;
	SDL_Event event;
	
	Vector2f playerVelocity;
	SDL_Rect player = {100,100,25,25};
	playerVelocity.x=playerVelocity.y=0;
	float playerSpeed = 2.5f;
	
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;
	
	Uint32 frameStart, frameTime;
	
	Object* o = generateObject(init, previousLocation);
	init = true;
	
	Game game;
	game.score = 0;
	
	timer = 60;
	int ticks = 0;
	
	// Loading the Font
	TTF_Init();
	TTF_Font* font = TTF_OpenFont("Minecraft.ttf", 36);
	SDL_Color white = { 0, 255, 0 };
	SDL_Color red = {255,0,0};
	SDL_Rect ScoreRectSize = { 0, 15, 0, 0 };
	SDL_Rect TimerRect = {0,15,0,0};
	SDL_Rect ScoreRect = {0,0,0,0};
	

	SDL_Texture* texture;
	SDL_Surface* surface;
	SDL_Texture* texture2;
	while (running) {
		frameStart = SDL_GetTicks();
		
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT)
			running = false;
		
		if (timer == 0) {
			// Redirect to the Score Screen
			currentScreen = 1;
		}
		
		if (currentScreen == 0) {
			if (ticks == 100) {
				--timer;
				ticks = 0;
			}
			
			if (isKeyPressed(SDL_SCANCODE_A))
				playerVelocity.x = -playerSpeed;
			if (isKeyPressed(SDL_SCANCODE_D))
				playerVelocity.x = playerSpeed;
			if (isKeyPressed(SDL_SCANCODE_S))
				playerVelocity.y = playerSpeed;
			if (isKeyPressed(SDL_SCANCODE_W))
				playerVelocity.y = -playerSpeed;
		
			player.x += playerVelocity.x;
			if (player.x+player.w >= SCREEN_WIDTH)
				player.x -= playerVelocity.x;
			else if (player.x <= 0)
				player.x -= playerVelocity.x;
			
			player.y += playerVelocity.y;
			if (player.y+player.h >= SCREEN_HEIGHT)
				player.y -= playerVelocity.y;
			else if (player.y <= 0)
				player.y -= playerVelocity.y;
		
			surface = TTF_RenderText_Blended(font, to_string(game.score).c_str(), white);
			texture = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_FreeSurface(surface);
			surface = TTF_RenderText_Blended(font, to_string(timer).c_str(), red);
			texture2 = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_FreeSurface(surface);
		
			SDL_QueryTexture(texture, nullptr, nullptr, &ScoreRectSize.w, &ScoreRectSize.h);
			SDL_QueryTexture(texture2, nullptr, nullptr, &TimerRect.w, &TimerRect.h);
			ScoreRectSize.x = SCREEN_WIDTH-ScoreRectSize.w-15;
			TimerRect.x = 25;
		
			if (checkCollision(o, player)) {
				o->destroyed = true;
				delete o;
				++game.score;
				playerSpeed += 0.1f;
				// cout << game.score << endl;
			
				o = generateObject(init, previousLocation);
			}
		} else if (currentScreen == 1) {
			string text = "Your Score Was "+to_string(game.score);
			surface = TTF_RenderText_Blended(font, text.c_str(), white);
			texture = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_FreeSurface(surface);
			
			SDL_QueryTexture(texture, nullptr, nullptr, &ScoreRect.w, &ScoreRect.h);
			ScoreRect.x = SCREEN_WIDTH/2-ScoreRect.w/2;
			ScoreRect.y = SCREEN_HEIGHT/2-ScoreRect.h/2;
		}
		
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		
		if (currentScreen == 0) {
			SDL_SetRenderDrawColor(renderer, 255, 191, 0, 255);
			SDL_RenderFillRect(renderer, &player);
			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
			if (!o->destroyed) { 
				SDL_Rect objectRect = { (int)o->location.x, (int)o->location.y, 15, 15 };
				SDL_RenderFillRect(renderer, &objectRect);
			}
		
			SDL_RenderCopy(renderer, texture, nullptr, &ScoreRectSize);
			SDL_RenderCopy(renderer, texture2, nullptr, &TimerRect);
		} else if (currentScreen == 1) {
			// Render the Score, Todo
			SDL_RenderCopy(renderer, texture, nullptr, &ScoreRect);
		}
		
		SDL_RenderPresent(renderer);
		
		playerVelocity.x = playerVelocity.y = 0;
		previousLocation = o->location;
		
		if (currentScreen == 0){
			++ticks;
			SDL_DestroyTexture(texture);
			SDL_DestroyTexture(texture2);
		} else if (currentScreen == 1) {
			SDL_DestroyTexture(texture);
		}
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
		object->location.x = getRandomInt(0, SCREEN_WIDTH-50);
		object->location.y = getRandomInt(0, SCREEN_HEIGHT-50);
	} else {
		if (previousLocation.x > SCREEN_WIDTH/2) {
			object->location.x = getRandomInt(0, SCREEN_WIDTH/2);
		} else {
			object->location.x = getRandomInt(SCREEN_WIDTH/2, SCREEN_WIDTH-50);
		}
		
		if (previousLocation.y > SCREEN_HEIGHT/2) {
			object->location.y = getRandomInt(0, SCREEN_HEIGHT-50);
		} else {
			object->location.y = getRandomInt(SCREEN_HEIGHT/2, SCREEN_HEIGHT-50);
		}
	}
	return object;
}
bool checkCollision(Object* object, SDL_Rect player) {
	unsigned int width = 25;
	unsigned int height = 25;
	float bottomA, bottomB;
	float topA, topB;
	float rightA, rightB;
	float leftA, leftB;
	
	leftA = object->location.x;
	topA = object->location.y;
	rightA = object->location.x+width;
	bottomA = object->location.y+height;
	
	topB = player.y;
	leftB = player.x;
	rightB = player.x+player.w;
	bottomB = player.y+player.h;
	
	if (bottomA <= topB)
		return false;
	if (topA >= bottomB)
		return false;
	if (rightA <= leftB)
		return false;
	if (leftA >= rightB)
		return false;
	
	return true;
}
int getRandomInt(int min, int max) {
	uniform_int_distribution<int> dist(min, max);
	return dist(rng);
}
double getRandomDouble(double min, double max) {
	uniform_real_distribution<double> dist(min, max);
	return dist(rng);
}