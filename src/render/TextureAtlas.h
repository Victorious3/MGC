#pragma once
#include <stdafx.h>
#include <unordered_set>

#include "render.h"

namespace render {

	class TextureAtlas final {
	public:
		~TextureAtlas() { destroy(); }

		// The sprite returned is only usable after load() is called
		// This reads the width & height of the image, has the side
		// effect of enforcing that we get a valid image path
		// TextureManager doesn't do this, it fails later
		Sprite& add_sprite(string sprite);

		// If you load a texture atlas the viewport and projection matrices
		// will become undefined, so make sure to reset those yourself
		void load();
		void destroy();

		// Invalidates the viewport & projection
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
		vector<GLuint> gl_textures;
		std::unordered_set<SpriteEntry, SpriteEntry::hash> sprites;
	};
}