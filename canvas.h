#pragma once

#include "stddef.h"

namespace mgc {
	class Sprite;

	class Canvas {
	public:
		SDL_Surface* draw_text_surface(TTF_Font* font, string text) const;
		GLuint create_texture(SDL_Surface * surface) const;
		void draw_texture(int x, int y, int w, int h, GLuint texture, SDL_Color color) const;
		void draw_surface(int x, int y, SDL_Surface* surface, SDL_Color color) const;
		void draw_text(int x, int y, TTF_Font* font, SDL_Color color, string text) const;
		void draw_sprite(Sprite* sprite, int x, int y);
	};
}
