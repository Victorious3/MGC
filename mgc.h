#pragma once

#include "stddef.h"

namespace mgc {

	// SDL window instance
	extern SDL_Window* window;
	// SDL renderer instance
	extern SDL_Renderer* renderer;

	int init();
	void destroy();
}