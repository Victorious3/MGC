#pragma once

#include <stdafx.h>

#include "Texture.h"

/*
constexpr Uint64 operator ""_sec(Uint64 val) {
	return val * 1000;
}

constexpr Uint64 operator ""_min(Uint64 val) {
	return val * 1000 * 60;
}
*/

namespace render {

	struct Color {
		union {
			struct {
				GLbyte r;
				GLbyte b;
				GLbyte g;
				GLbyte a;
			};
			GLbyte components[4];
		};
		operator GLbyte*() { return components; }
	};

	typedef SDL_Rect  Rectangle;
	typedef SDL_Point Point;

	constexpr Color color(GLbyte r, GLbyte g, GLbyte b) {
		return Color { r, g, b, (GLbyte)255};
	}

	constexpr Color color(GLbyte r, GLbyte g, GLbyte b, GLbyte a) {
		return Color { r, g, b, a };
	}

	constexpr Color color(Uint32 rbg) {
		return Color { (GLbyte)(rbg >> 16), (GLbyte)(rbg >> 8), (GLbyte)rbg, (GLbyte)255 };
	}

	constexpr Color color_a(Uint32 rbga) {
		return Color { (GLbyte)(rbga >> 24), (GLbyte)(rbga >> 16), (GLbyte)(rbga >> 8), (GLbyte)rbga };
	}

	constexpr Rectangle rectangle(int w, int h, int x = 0, int y = 0) {
		return Rectangle { x, y, w, h };
	}

	namespace colors {
		constexpr Color TRANSPARENT = color_a(0);
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

	inline void try_throw_gl_error(const string& error_string) {
		if (GLenum error = glGetError()) {
			throw RUNTIME_ERROR(error_string + ": "s + string(reinterpret_cast<const char*>(gluErrorString(error))) + " ("s + std::to_string(error) + ")");
		}
	}

	GLuint allocate_texture(Uint w, Uint h, const void* pBuffer = nullptr);

	void gl_object_label(const string& str, GLenum identifier, GLuint name);
	
	GLuint create_framebuffer(GLuint texture_attachment);

	// Returns the current fbo
	GLuint bind_framebuffer(GLuint fbo);
};
