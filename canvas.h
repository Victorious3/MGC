#pragma once

#include "stddef.h"

typedef SDL_Color Color;

namespace mgc {

	constexpr Color color(Uint8 r, Uint8 g, Uint8 b) {
		return Color{ r, g, b, 255 };
	}

	constexpr Color color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
		return Color{ r, g, b, a };
	}

	constexpr Color color(Uint32 rbg) {
		return Color{ (Uint8)(rbg >> 16), (Uint8)(rbg >> 8), (Uint8)rbg, 255 };
	}

	constexpr Color color_a(Uint32 rbga) {
		return Color{ (Uint8)(rbga >> 24), (Uint8)(rbga >> 16), (Uint8)(rbga >> 8) };
	}

	
	namespace colors {
		constexpr Color BLACK = color(0x000000);
		constexpr Color WHITE = color(0xFFFFFF);
		constexpr Color RED = color(0xFF0000);
		constexpr Color BLUE = color(0x00FF00);
		constexpr Color GREEN = color(0x0000FF);
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
