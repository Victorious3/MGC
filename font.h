#pragma once
#include "stddef.h"
#include "texture.h"

namespace mgc {
	class Font final {
	public:
		Font(string file);
		
	private:
		// Sprite
		Texture texture;

		Uint64 map_width;
		Uint64 map_height;
		Uint64 cell_width;
		Uint64 cell_height;

		char start_char;
		char end_char;

		vector<Uint8> char_sizes;
	};
}