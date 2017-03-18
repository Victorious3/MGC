#pragma once
#include <stdafx.h>

namespace render {
	class Canvas;
	class Texture;

	class TextureManager {
	public:
		void load_all();
		void destroy_all();
		void reload();

		// Force loading a texture
		virtual void load(Texture& texture);
		// Force destroying a texture
		virtual void destroy(Texture& texture);

		// Store a reference to the texture for later use
		void store(Texture& texture);
	protected:
		vector<Texture*> textures;
	};

	class TextureAtlas {
	public:
		TextureAtlas(string cache_file) : cache_file(cache_file) {}

	private:
		const string cache_file;
	};

	// Default for drawing to the screen, destroys textures on program exit
	extern TextureManager texture_manager;

	struct Sprite {
		Sprite(Uint w, Uint h, float umin = 0, float vmin = 0, float umax = 1, float vmax = 1)
			: w(w), h(h), umin(umin), vmin(vmin), umax(umax), vmax(vmax) {}

		Uint w, h;
		float umin, vmin, umax, vmax;
		
		GLuint gl_texture = 0;
	};

	class Texture : public Sprite {
	public:
		Texture(string path, TextureManager& mgr = texture_manager) : Sprite(0, 0), path(path), mgr(mgr) {}

		inline void load() { if (!is_loaded()) mgr.load(*this); }
		inline void destroy() { mgr.destroy(*this); }
		inline bool is_loaded() const { return gl_texture != 0; }

	public:
		const string path;
	private:
		TextureManager& mgr;
	};
}