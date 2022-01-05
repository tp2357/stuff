#include <iostream>
#include <vector>
#include <cmath>
#include <windows.h>
#include <SDL2/SDL.h>

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const int g_dx = 20;

void grid() {
	int i, j;
	SDL_SetRenderDrawColor(renderer, 0, 0xff, 0, 0xff);
	for (i=0; i<=WINDOW_WIDTH/g_dx; i++) {
		for (j=0; j<=WINDOW_HEIGHT/g_dx; j++) {
			SDL_RenderDrawLine(renderer, g_dx*i, 0, g_dx*i, WINDOW_HEIGHT);
			SDL_RenderDrawLine(renderer, 0, g_dx*j, WINDOW_WIDTH, g_dx*j);
		}
	}
	// SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
}

class Vec2 {
public:
	int x, y;
	SDL_Color color;
	Vec2() {
		x = 0; y = 0;
		color.r = 0; color.g = 0xff; color.b = 0;
	}
	Vec2(int x, int y) {
		this->x = x; this->y = y;
		color.r = 0; color.g = 0xff; color.b = 0;
	}
	void render() {
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 0xff);
		SDL_Rect rect = {g_dx * x, WINDOW_HEIGHT - g_dx * y - g_dx, g_dx, g_dx};
		SDL_RenderFillRect(renderer, &rect);
	}
};

enum S_DIRECTION {
	D_UP,
	D_DOWN,
	D_LEFT,
	D_RIGH
};


class Snake {
	Vec2 head;
	std::vector<Vec2> body;
	S_DIRECTION direction;
	int ft;
public:
	Snake() {
		ft = 0;
		direction = D_UP;

		body.push_back(Vec2(0, -1));
		body.push_back(Vec2(0, -2));
		body.push_back(Vec2(0, -3));
		body.push_back(Vec2(0, -4));
	}
	void render() {
		head.render();
		std::vector<Vec2>::iterator it = body.begin();
		while (it != body.end()) {
			it->render();
			it++;
		}
		ft++;
		if (ft % 2 == 0) {
			move();
		}
	}
	void set_direction(S_DIRECTION d) {
		direction = d;
	}
	void move() {
		int ox = head.x, oy = head.y;
		switch (direction) {
			case D_UP: head.y++;
			break;
			case D_DOWN: head.y--;
			break;
			case D_LEFT: head.x--;
			break;
			case D_RIGH: head.x++;
			break;
		}
		int size = body.size();
		body.at(size-1).x = ox;
		body.at(size-1).y = oy;
		int i;
		for (i=0; i<size-1; i++) {
			body.at(i).x = body.at(i+1).x;
			body.at(i).y = body.at(i+1).y;
		}
	}
	void update(const Vec2& food) {
	}
};


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cerr << SDL_GetError() << std::endl;
		return -1;
	}
	window = SDL_CreateWindow("SDL2",
						SDL_WINDOWPOS_UNDEFINED,
						SDL_WINDOWPOS_UNDEFINED,
						WINDOW_WIDTH,
						WINDOW_HEIGHT,
						SDL_WINDOW_OPENGL);
	if (window == NULL) {
		std::cerr << SDL_GetError() << std::endl;
		return -1;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (renderer == NULL) {
		std::cerr << SDL_GetError() << std::endl;
		return -1;
	}

	SDL_Event event;
	bool quit = false;

	Snake snake;
	Vec2 food(10, 10);
	food.color.g = 0;
	food.color.r = 0xff;

	while (!quit) {
			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_QUIT) quit = true;
				if (event.type == SDL_KEYDOWN) {
					switch (event.key.keysym.sym) {
						case SDLK_UP: snake.set_direction(D_UP);
						break;
						case SDLK_DOWN: snake.set_direction(D_DOWN);
						break;
						case SDLK_RIGHT: snake.set_direction(D_RIGH);
						break;
						case SDLK_LEFT: snake.set_direction(D_LEFT);
						break;
					}
				}
			}
			SDL_Delay(1000 / 60);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
			SDL_RenderClear(renderer);
			grid();

			snake.render();
			food.render();
			SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
