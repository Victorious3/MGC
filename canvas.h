#pragma once

#include "stddef.h"

namespace mgc {
	class Canvas {
	public:
		void draw_text(int x, int y, TTF_Font *font, SDL_Color color, string text);
	};
}
