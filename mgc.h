#pragma once

#include "stddef.h"

namespace mgc {

	// SDL window instance
	extern SDL_Window* window;
	// GL context
	extern SDL_GLContext context;

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
		bool state;

		string text;
		Sint32 cur;
	};

	struct Graphics {
		TTF_Font *debugFont;
		float actualFramerate;
		Uint32 msPerFrame;
		Uint32 frameCounter;
		Uint32 lastCountTime;
	};

	void init_sdl();
	void init_lua();

	void sdl_event();
	void toggle_fullscreen();
	void render();
	void run();

	void destroy_sdl();
	void destroy_lua();
}