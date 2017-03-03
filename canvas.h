#pragma once

#include "stddef.h"

typedef SDL_Color Color;

namespace mgc {

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

	class Sprite;

	class Canvas {
	public:
		// TODO These might want to be somewhere else
		inline void enable_gl_texture() const {
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		inline void disable_gl_texture() const {
			glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);
		}

		void draw_gl_texture(GLuint texture, int x, int y, int w, int h, Color color = colors::WHITE) const;
		void draw_gl_texture(GLuint texture, int x, int y, int w, int h, float umin, float vmin, float umax, float vmax, Color color = colors::WHITE) const;
	};
}
