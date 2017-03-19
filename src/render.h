#pragma once

#include <stdafx.h>

typedef SDL_Color Color;

constexpr Uint64 operator ""_sec(Uint64 val) {
	return val * 1000;
}

constexpr Uint64 operator ""_min(Uint64 val) {
	return val * 1000 * 60;
}

namespace render {

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

	inline float linear_interpolatef(const int& start, const int& end) {
		Uint32 ticks = SDL_GetTicks();
		if (ticks <= start) return 0;
		else if (ticks >= end) return 1;
		else {
			return (ticks - start) / (end - start);
		}
	}

	inline float linear_interpolatef(const float& progress, const float& start_val, const float& end_val) {
		return progress * (start_val - end_val) + start_val;
	}

	inline void linear_interpolatef(float& target, const int& start_time, const int& end_time, const float& start_val, const float& end_val) {
		target = linear_interpolatef(start_time, end_time) * (end_val - start_val) + start_val;
	}

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

	struct Sprite;

	void draw_sprite(const Sprite& sprite, int x, int y, Color color = colors::WHITE);
	void draw_gl_texture(GLuint texture, int x, int y, int w, int h, Color color = colors::WHITE);
	void draw_gl_texture(GLuint texture, int x, int y, int w, int h, float umin, float vmin, float umax, float vmax, Color color = colors::WHITE);
};
