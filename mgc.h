#pragma once

#include "stddef.h"

namespace mgc {

	// SDL window instance
	extern SDL_Window* window;
	// GL context
	extern SDL_GLContext context;
	extern float scale;

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
		TTF_Font *font_debug;
		float framerate_actual;
		Uint32 frame_delay_ms;
		Uint32 frame_counter;
		Uint32 last_count_taken;
	};

	struct Timing {
		Uint32 tick_counter;
		Uint32 tick_last;
		Uint32 tick_delay_ms;
	};

	void init_sdl();
	void init_lua();

	void sdl_event();
	void toggle_fullscreen();
	void render();
	void update();
	void run();

	void destroy_sdl();
	void destroy_lua();
}