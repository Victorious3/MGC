#pragma once
#include <stdafx.h>

namespace mgc {
	class Canvas;
	class Texture;

	class TextureManager {
	public:
		void load_all();
		void destroy_all();

		// Force loading a texture
		void load(Texture& texture);
		// Force destroying a texture
		void destroy(Texture& texture);

		// Store a reference to the texture for later use
		void store(Texture& texture);
	protected:
		vector<Texture*> textures;
	};

	// Default for drawing to the screen, destroys textures on program exit
	extern TextureManager texture_manager;

	class Texture {
	public:
		Texture(string path, TextureManager& mgr = texture_manager);

		inline void load() { if (!is_loaded()) mgr.load(*this); }
		inline void destroy() { mgr.destroy(*this); }
		inline bool is_loaded() const { return gl_texture != 0; }

		virtual void draw(const Canvas& canvas, int x, int y);
		
	public:
		// Width and height aren't populated until the texture is loaded
		Uint w, h;
		GLuint gl_texture = 0;
		const string path;

	private:
		TextureManager& mgr;
	};
}