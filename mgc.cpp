#include "stddef.h"

#include "mgc.h"

namespace mgc {

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	Mouse mouse;
	Keyboard key;

	bool running = true;
	bool fullscreen = false;

	void run() {
		while (running) {
			// Handle sdl events
			sdl_event();
		}
	}

	void sdl_event() {
		SDL_Event event;

		mouse.clicked = key.typed = mouse.moved = false;
		mouse.wheel = mouse.xrel = mouse.yrel = 0;

		while (SDL_PollEvent(&event)) {

			bool mdown = mouse.down;
			bool kdown = key.down;

			switch (event.type) {
			case SDL_QUIT:
				running = false;
				break;
			case SDL_MOUSEMOTION:
				mouse.x = event.motion.x;
				mouse.y = event.motion.y;
				mouse.xrel = event.motion.xrel;
				mouse.yrel = event.motion.yrel;
				mouse.moved = mouse.xrel || mouse.yrel;
				break;
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				mouse.down = event.button.state;
				mouse.clicked = !mdown && mouse.down;
				break;
			case SDL_MOUSEWHEEL:
				mouse.wheel = event.wheel.y;
				break;
			case SDL_KEYDOWN:
			case SDL_KEYUP: 
				key.down = event.key.state;
				key.cur = event.key.keysym.sym;
				key.changed = kdown != key.down;

				switch (event.key.keysym.sym) {
				case SDLK_UP: key.key_up = key.typed; break;
				case SDLK_DOWN: key.key_down = key.typed; break;
				case SDLK_LEFT: key.key_left = key.typed; break;
				case SDLK_RIGHT: key.key_right = key.typed; break;
				}
				break;
			case SDL_TEXTINPUT:
				key.typed = true;
				key.text = event.text.text;
				break;
			}
		}

		// Update game here

		if (key.typed)
			cout << "Keystroke: " << key.cur << ", " << key.text << endl;
		if (mouse.clicked)
			cout << "Mouse clicked: " << mouse.x << ", " << mouse.y << endl;
		if (mouse.wheel)
			cout << "Mouse wheel moved: " << mouse.wheel << endl;
		if (mouse.moved)
			cout << "Mouse moved: " << mouse.x << ", " << mouse.y << endl;
	}

	void init_sdl() {
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS)) {
			throw runtime_error("SDL_Init Error: "s + SDL_GetError());
		}
		SDL_Log("SDL initialized successfully");

		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		window = SDL_CreateWindow(
			constants::APP_NAME.c_str(), // title
			SDL_WINDOWPOS_CENTERED,	// xpos
			SDL_WINDOWPOS_CENTERED, // ypos
			constants::SCR_WIDTH,	// width
			constants::SCR_HEIGHT,	// height
			SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL // flags, we force OpenGL
		);

		if (!window) {
			throw runtime_error("SDL_CreateWindow Error: "s + SDL_GetError());
		}

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

		if (!renderer) {
			throw runtime_error("SDL_CreateRenderer Error: "s + SDL_GetError());
		}

		// Test
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
		SDL_RenderDrawLine(renderer, 0, 0, constants::SCR_WIDTH, constants::SCR_HEIGHT);
		SDL_RenderPresent(renderer);
	}

	void destroy_sdl() {
		if (window) SDL_DestroyWindow(window);
		if (renderer) SDL_DestroyRenderer(renderer);
		SDL_Quit();
	}

	void init_lua() {

	}
	
	void destroy_lua() {

	}
}
