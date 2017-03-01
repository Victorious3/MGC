#pragma once

#include "stddef.h"

typedef SDL_Color Color;

namespace mgc {
	class Sprite;

	class Canvas {
	public:
		SDL_Surface* draw_text_surface(TTF_Font& font, string text) const;
		void draw_texture(int x, int y, int w, int h, Uint texture, Color color) const;
		void draw_surface(int x, int y, SDL_Surface& surface, Color color) const;
		void draw_text(int x, int y, TTF_Font& font, Color color, string text) const;
	};

	constexpr Color color(Uint8 r, Uint8 g, Uint8 b);
	constexpr Color color(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	constexpr Color color(Uint32 rbg);
	constexpr Color color_a(Uint32 rbga);

	namespace colors {
		const Color BLACK = color(0x000000);
		const Color WHITE = color(0xFFFFFF);
		const Color RED = color(0xFF0000);
		const Color BLUE = color(0x00FF00);
		const Color GREEN = color(0x0000FF);
	};
}
