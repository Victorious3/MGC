#include "stddef.h"

#include "mgc.h"

namespace mgc {

	SDL_Window* window;
	SDL_Renderer* renderer;

	int init() {
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS)) {
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_Init Error: %s \n", SDL_GetError());
			return 1;
		}
		SDL_Log("SDL initialized successfully");

		window = SDL_CreateWindow(
			constants::APP_NAME.c_str(), // title
			SDL_WINDOWPOS_CENTERED,	// xpos
			SDL_WINDOWPOS_CENTERED, // ypos
			constants::SCR_WIDTH,	// width
			constants::SCR_HEIGHT,	// height
			SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL // flags, we force OpenGL
		);

		if (!window) {
			SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "SDL_CreateWindow Error: %s \n", SDL_GetError());
			destroy();
			return 1;
		}

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

		if (!renderer) {
			SDL_LogError(SDL_LOG_CATEGORY_RENDER, "SDL_CreateRenderer Error: %s \n", SDL_GetError());
			destroy();
		}

		// Test
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
		SDL_RenderDrawLine(renderer, 0, 0, constants::SCR_WIDTH, constants::SCR_HEIGHT);
		SDL_RenderPresent(renderer);

		// Wait
		int tmp;
		cin >> tmp;

		destroy();
	}

	void destroy() {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Shutting down!");
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
	}
}
