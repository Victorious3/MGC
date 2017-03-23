#include <stdafx.h>
#include "TextureAtlas.h"

namespace render {
	Sprite& TextureAtlas::add_sprite(string sprite) {
		Uint32 w, h;
		read_img_dim_png(&w, &h, sprite);
		sprites.push_back(Sprite(w, h));
		return sprites.back();
	}

	void TextureAtlas::load() {
		/*if (!refresh_cache && !cache_file.empty()) {
		// First we try to read the cache
		// The cache file is a simple binary file that stores the computed positions of the sprites
		// and the size of the cache file
		ifstream cachef(cache_file + ".dat", std::ios_base::binary);
		if (cachef.good()) {
		// TODO

		return;
		}
		}*/



		// We have to rebuild the cache
	}

	void TextureAtlas::destroy() {
		glDeleteTextures(1, &texture.gl_texture);
		refresh_cache = true;
	}

}