#pragma once

#include "stddef.h"

namespace mgc {

	// SDL window instance
	extern SDL_Window* window;
	// SDL renderer instance
	extern SDL_Renderer* renderer;

	struct Mouse {
		int x;
		int y;
		int xrel;
		int yrel;
		int wheel;

		bool down;
		bool clicked;
		bool moved;
	};

	struct Keyboard {
		bool key_up;
		bool key_down;
		bool key_left;
		bool key_right;

		bool typed;
		bool down;
		bool changed;

		string text;
		Sint32 cur;
	};

	void init_sdl();
	void init_lua();

	void sdl_event();

	void run();

	void destroy_sdl();
	void destroy_lua();
}