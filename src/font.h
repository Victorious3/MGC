#pragma once
#include <stdafx.h>

#include "texture.h"
#include "render.h";

namespace mgc {
	class Font final {
	public:
		Font(string file);

		Uint char_width(const char& c) const;
		Uint draw_char(char c, int x, int y, render::Color color = render::colors::WHITE);
		Uint draw_string(string s, int x, int y, render::Color color = render::colors::WHITE);
		
	private:
		// Sprite
		render::Texture& texture;

		Uint32 map_width;
		Uint32 map_height;
		Uint32 cell_width;
		Uint32 cell_height;

		char start_char;

		Uint8 char_sizes[256];

		Uint draw_char_internal(char c, int x, int y, render::Color color) const;
	};
}