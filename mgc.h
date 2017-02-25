#pragma once

#include "stddef.h"

namespace mgc {

	// SDL window instance
	extern SDL_Window* window;
	// SDL renderer instance
	extern SDL_Renderer* renderer;

	void init_sdl();
	void init_lua();

	void sdl_event();

	void run();

	void destroy_sdl();
	void destroy_lua();
}