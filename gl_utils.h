#pragma once

#include "stddef.h"

namespace gl_utils {
	void draw_text(int x, int y, TTF_Font *font, SDL_Color color, string text);
}