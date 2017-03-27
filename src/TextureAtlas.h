#pragma once
#include <stdafx.h>

#include "render.h"

namespace render {

	class TextureAtlas final {
	public:
		// An empty string means no cache file is used, this is explicit to avoid erros
		TextureAtlas(string cache_file) : cache_file(cache_file) {}

		// The sprite returned is only usable after load() is called
		// This reads the width & height of the image, has the side
		// effect of enforcing that we get a valid image path
		// TextureManager doesn't do this, it fails later
		Sprite& add_sprite(string sprite);

		void load();
		void destroy();

		inline void reload() {
			destroy();
			load();
		}

	private:
		struct SpriteEntry {
			Sprite sprite;
			string path;
			int x, y;
		};

		bool refresh_cache = false; // Flag to rewrite the cache, set when destroy is called
		Sprite texture;
		const string cache_file;
		std::list<SpriteEntry> sprites;
	};
}