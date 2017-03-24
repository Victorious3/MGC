#pragma once

#include <stdafx.h>

#include "Texture.h"

constexpr Uint64 operator ""_sec(Uint64 val) {
	return val * 1000;
}

constexpr Uint64 operator ""_min(Uint64 val) {
	return val * 1000 * 60;
}

namespace render {

	typedef SDL_Color Color;
	typedef SDL_Rect  Rectangle;
	typedef SDL_Point Point;

	constexpr Color color(Uint8 r, Uint8 g, Uint8 b) {
		return Color { r, g, b, 255 };
	}

	constexpr Color color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
		return Color { r, g, b, a };
	}

	constexpr Color color(Uint32 rbg) {
		return Color { (Uint8)(rbg >> 16), (Uint8)(rbg >> 8), (Uint8)rbg, 255 };
	}

	constexpr Color color_a(Uint32 rbga) {
		return Color { (Uint8)(rbga >> 24), (Uint8)(rbga >> 16), (Uint8)(rbga >> 8) };
	}

	namespace colors {
		constexpr Color BLACK = color(0x000000);
		constexpr Color WHITE = color(0xFFFFFF);
		constexpr Color RED = color(0xFF0000);
		constexpr Color BLUE = color(0x00FF00);
		constexpr Color GREEN = color(0x0000FF);
	};

	struct GLVars {
		GLint max_tex_size;
	};
	extern GLVars glvars;
	
	void init_glvars();

	// TODO These might want to be somewhere else
	inline void enable_gl_texture() {
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	inline void disable_gl_texture() {
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
	}

	GLuint allocate_texture(Uint w, Uint h, const void* pBuffer = nullptr);

	void draw_sprite(const Sprite& sprite, int x, int y, Color color = colors::WHITE);
	void draw_texture(Texture& sprite, int x, int y, Color color = colors::WHITE);

	void draw_gl_texture(GLuint texture, int x, int y, int w, int h, Color color = colors::WHITE);
	void draw_gl_texture(GLuint texture, int x, int y, int w, int h, float umin, float vmin, float umax, float vmax, Color color = colors::WHITE);
};
