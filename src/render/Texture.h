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

		operator GLuint() { return gl_texture; }
	};

	inline Texture& add_texture(string path, TextureManager& mgr = texture_manager) {
		return texture_manager.add_texture(path);
	}

	GLuint create_texture(const string& path, Uint* w, Uint* h);

	// Only reads the file header. Checks for validity, so feel free to pass garbage.
	void read_img_dim_png(Uint32* w, Uint32* h, const string& file);

	class TextureAtlas;
	class Texture final : public Sprite {
	private:
		friend TextureManager;
		Texture(string path, TextureManager& mgr) : Sprite(0, 0), path(path), mgr(mgr) {}

	public:
		void load() { if (gl_texture == 0) mgr.load(*this); }

	public:
		const string path;
	private:
		TextureManager& mgr;
	};
}