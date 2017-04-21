#pragma once
#include <stdafx.h>
#include <unordered_set>

#include "render.h"

namespace render {

	class TextureAtlas final {
	public:
		TextureAtlas(string cache_file = "") : cache_file(cache_file) {}
		~TextureAtlas() { destroy(); }

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
			mutable Sprite sprite;
			string path;
			mutable int x, y;

			bool operator==(const SpriteEntry& e) const;
			static struct hash { size_t operator()(const SpriteEntry& e) const; };
		};

		void create_bucket(std::list<const SpriteEntry*>& sprites);

		bool refresh_cache = false; // Flag to rewrite the cache, set when destroy is called
		vector<GLuint> gl_textures;
		const string cache_file;

		std::unordered_set<SpriteEntry, SpriteEntry::hash> sprites;
	};
}