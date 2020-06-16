// directory: /Users/navyanpahwa/documents/dev/games/minigame_1
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <iostream>
#include <cmath>
#include <random>
#include <cstdlib>
#include <cstdio>
#include <vector>
using namespace std;

typedef struct {
	float x, y;
} Vector2f;
typedef struct {
	float x, y, w, h;
} Vector4f;
typedef struct {
	float r, g, b, a;
} Color;

class Game {
public:
	unsigned int score = 0;
	bool lost = false;
	Vector2f playerSpawnPoint;
	
	Game() {  }
};

class Ball {
public:
	Vector2f position;
	unsigned int width, height;
	float fallSpeed;
	
	bool isDestroyed = false;
	
	Ball(Vector2f location, float fallspeed) {
		this->position = location;
		this->width = this->height = 25;
		this->fallSpeed = fallspeed;
	}
	void renderAndUpdate (SDL_Renderer* ren) {
		if (!isDestroyed) {
			SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
			SDL_Rect rect = {(int)this->position.x, (int)this->position.y, (int)this->width, (int)this->height};
			SDL_RenderFillRect(ren, &rect);
		
			this->position.y += this->fallSpeed;
		} else {
			;
		}
	}
	void destroy() {
		this->isDestroyed = true;
	}
};

static random_device rd;
static default_random_engine dre(rd());

int randomInt(int min, int max);

void renderObject(SDL_Renderer* ren, Vector4f rect, Color color); // render the object
bool isKeyPressed(const Uint8* keyboardState, SDL_Scancode key); // is key pressed?

vector<Ball*> ballSpawner(SDL_Renderer* ren, unsigned int xRange, unsigned int yRange, unsigned int maxCount);
void renderAndUpdateBalls(vector<Ball*> balls, SDL_Renderer* ren);

int main(int argc, char* argv[]) {
	const int screenWidth = 640;
	const int screenHeight = 480;
	const std::string screenTitle = "Minigame";
	
	SDL_Init(SDL_INIT_EVERYTHING);
	
	SDL_Window* mWindow = SDL_CreateWindow(screenTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
	SDL_Renderer* mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_PRESENTVSYNC);
	SDL_Event e;
	const Uint8* keyboardState = SDL_GetKeyboardState(0);
	bool running = true;
	
	Game game;
	game.playerSpawnPoint.x = 10;
	game.playerSpawnPoint.y = screenHeight-50;
	
	Vector4f playerRect = { game.playerSpawnPoint.x, game.playerSpawnPoint.y, 150, 25 };
	const float playerSpeed = 2.5;
	float playerVelX = 0;

	const float BallFallSpeed = 1.5f;
	
	Uint32 frameStart, frameTime;
	const int FPS = 30;
	const int FrameDelay = 1000/FPS;
	
	// Generate the Balls Right Here
	vector<Ball*> balls = ballSpawner(mRenderer, screenWidth, 0, 4);
	cout << balls.size() << endl;
	
	while (running) {
		frameStart = SDL_GetTicks();
		
		SDL_PollEvent(&e);
		if (e.type == SDL_QUIT)
			running = false;
		
		// player movement 
		if (isKeyPressed(keyboardState, SDL_SCANCODE_A))
			playerVelX = -playerSpeed; // move left
		else if (isKeyPressed(keyboardState, SDL_SCANCODE_D))
			playerVelX = playerSpeed; // move right
		
		playerRect.x += playerVelX;
		
		// Player Logic
		if (playerRect.x >= screenWidth-playerRect.w)
			playerRect.x -= playerVelX;
		else if (playerRect.x <= 0)
			playerRect.x -= playerVelX;
			
		SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
		SDL_RenderClear(mRenderer);
		
		renderAndUpdateBalls(balls, mRenderer);
		renderObject(mRenderer, playerRect, { 255, 255, 255, 255 });
		
		SDL_RenderPresent(mRenderer);
		
		playerVelX = 0;
		
		frameTime = SDL_GetTicks() - frameStart;
		if (FrameDelay > frameTime)
			SDL_Delay(FrameDelay-frameTime);
	}
	
	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(mRenderer);
	SDL_Quit();
	
	for (unsigned int i = 0; i < balls.size(); ++i)
		delete balls[i];
	return 0;
}

void renderObject(SDL_Renderer* ren, Vector4f rect, Color color) {
	SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, color.a);
	SDL_Rect sdlrect = { (int)rect.x,(int)rect.y,(int)rect.w,(int)rect.h };
	SDL_RenderFillRect(ren, &sdlrect);
}

bool isKeyPressed(const Uint8* keyboardState, SDL_Scancode key) {
	if (keyboardState)
		if (keyboardState[key])
			return true;
	return false;
}

vector<Ball*> ballSpawner(SDL_Renderer* ren, unsigned int xRange, unsigned int yRange, unsigned int maxBalls) {
	vector<Ball*> balls;
	Vector2f prev;
	for (unsigned int i = 0; i < maxBalls; ++i) {
		Ball* ball = nullptr;
		Vector2f location;
		location.y = randomInt(-150, -25);
		if (i==0)
			location.x=randomInt(0,xRange-50);
		else 
			if (prev.x>xRange/2)
				location.x=randomInt(0,xRange/2);
			else
				location.x=randomInt(xRange/2+50,xRange-50);
			
		ball = new Ball(location, 1.5f);
		prev = location;
		balls.push_back(ball);
	}
	return balls;
}

int randomInt(int min, int max) {
	uniform_int_distribution<int> dist(min,max);
	return dist(dre);
}

void renderAndUpdateBalls(vector<Ball*> balls, SDL_Renderer* ren) {
	for (unsigned int i = 0; i < balls.size(); ++i) {
		balls[i]->renderAndUpdate(ren);
	}
}

// up next 
// balls spawn from random points
// collision detection
// logic
//		if ball pasts the paddle without touching it, the player loses, game done
//		if player touches the ball, it is destroyed.

// Tweaks to make 
// 		Divide the Display into sub sections
//			Make Sure the Ball Spawns in each sub section so that they don't drift off.