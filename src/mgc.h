#pragma once
#include <stdafx.h>

#include "render.h"
#include "texture.h"

namespace mgc {

	// SDL window instance
	extern SDL_Window* window;
	// GL context
	extern SDL_GLContext context;

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
		Uint64 last_count_taken;
		Uint64 render_delta_ms;
	};

	struct Timing {
		Uint64 time_last;
		Uint64 time_delta;
		Uint64 ticks;
		Uint64 ticks_delta;
		Uint64 ticks_delta_ms;
		Uint64 tick_delay_ms;
	};

	void init();

	void toggle_fullscreen();
	void run();

	void update();
	void tick();
	void render();

	void destroy();
}