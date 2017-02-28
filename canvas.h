#pragma once

#include "stddef.h"

namespace mgc {
	class Sprite;

	class Canvas {
	public:
		SDL_Surface* draw_text_surface(TTF_Font* font, string text) const;
		void draw_surface(int x, int y, SDL_Surface* surface, SDL_Color color) const;
		void draw_text(int x, int y, TTF_Font* font, SDL_Color color, string text) const;
		void draw_sprite(const Sprite* sprite, int x, int y) const;
	};
}
