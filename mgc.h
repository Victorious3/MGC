#pragma once

#include "stddef.h"

namespace mgc {

	// SDL window instance
	extern SDL_Window* window;
	// SDL renderer instance
	extern SDL_Renderer* renderer;

	class Mouse {
	public:
		int x;
		int y;
		int xrel;
		int yrel;
		int wheel;

		bool down;
		bool clicked;
		bool moved;
	};

	class Keyboard {
	public:
		bool up;
		bool down;
		bool left;
		bool right;

		bool typed;

		string text;
		Sint32 cur;
	};

	extern bool running;

	void init_sdl();
	void init_lua();

	void sdl_event();

	void run();

	void destroy_sdl();
	void destroy_lua();
}