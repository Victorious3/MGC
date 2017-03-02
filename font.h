#pragma once
#include "stddef.h"
#include "texture.h"
#include "canvas.h"

namespace mgc {
	class Font final {
	public:
		Font(string file);

		Uint char_width(char c);
		Uint draw_char(Canvas& canvas, char c, int x, int y, Color color = colors::WHITE);
		Uint draw_string(Canvas& canvas, string s, int x, int y, Color color = colors::WHITE);
		
	private:
		// Sprite
		Texture texture;

		Uint32 map_width;
		Uint32 map_height;
		Uint32 cell_width;
		Uint32 cell_height;

		char start_char;

		Uint8 char_sizes[256];

		Uint draw_char_internal(Canvas& canvas, char c, int x, int y, Color color);
	};
}