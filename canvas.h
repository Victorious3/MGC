#pragma once

#include "stddef.h"

namespace mgc {
	class Canvas {
	public:
		SDL_Surface* draw_text_surface(TTF_Font* font, string text);
		void draw_surface(int x, int y, SDL_Surface* surface, SDL_Color color);
		void draw_text(int x, int y, TTF_Font* font, SDL_Color color, string text);
	};
}
