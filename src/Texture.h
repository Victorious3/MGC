#pragma once
#include <stdafx.h>

namespace render {
	
	class Texture;
	class TextureManager {
	public:
		TextureManager() {};
		virtual ~TextureManager() {};
		TextureManager(const TextureManager&) = delete;

		void load_all();
		void destroy_all();
		
		inline void reload() { 
			destroy_all(); 
			load_all(); 
		}

		// Force loading a texture
		virtual void load(Texture& texture);
		// Force destroying a texture
		virtual void destroy(Texture& texture);

		// Returns a (cached) texture instance for the given path
		Texture& add_texture(string path);
	protected:
		std::unordered_map<string, Texture> textures;
	};

	// Default for drawing to the screen, destroys textures on program exit
	extern TextureManager texture_manager;

	class Sprite {
	public:
		Sprite(Uint w = 0, Uint h = 0, float umin = 0, float vmin = 0, float umax = 1, float vmax = 1)
			: w(w), h(h), umin(umin), vmin(vmin), umax(umax), vmax(vmax) {}

		Uint w, h;
		float umin, vmin, umax, vmax;
		
		GLuint gl_texture = 0;
	};

	inline Texture& add_texture(string path, TextureManager& mgr = texture_manager) {
		return texture_manager.add_texture(path);
	}

	// Only reads the file header. Checks for validity, so feel free to pass garbage.
	void read_img_dim_png(Uint32* w, Uint32* h, const string& file);

	class TextureAtlas;
	class Texture final : public Sprite {
	private:
		friend TextureManager;
		friend TextureAtlas;
		Texture(string path, TextureManager& mgr) : Sprite(0, 0), path(path), mgr(mgr) {}

	public:
		void load() { if (gl_texture == 0) mgr.load(*this); }

	public:
		const string path;
	private:
		TextureManager& mgr;
	};

	class TextureAtlas final {
	public:
		// The cache_file doesn't have a file extension, it generates a bmp/dat file combo
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
		bool refresh_cache = false; // Flag to rewrite the cache, set when destroy is called
		Sprite texture;
		const string cache_file;
		std::list<Sprite> sprites {32};
	};

}