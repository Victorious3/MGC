#pragma once
#include <stdafx.h>

#include "canvas.h"
#include "texture.h"

namespace mgc {

	// SDL window instance
	extern SDL_Window* window;
	// GL context
	extern SDL_GLContext context;
	extern Canvas canvas;
	
	extern float scale;
	extern Uint xoff;
	extern Uint yoff;

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

	struct Graphics {
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

	void init();

	void toggle_fullscreen();
	void run();

	void destroy();
}