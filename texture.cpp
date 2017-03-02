#include "stddef.h"

#include "texture.h"
#include "canvas.h"

namespace mgc {

	TextureManager texture_manager;

	void TextureManager::load_all() {
		for (Texture* t : textures) {
			load(*t);
		}
	}

	void TextureManager::destroy_all() {
		vector<GLuint> glt(textures.size());
		for (Uint i = 0; i < textures.size(); i++) {
			glt[i] = textures[i]->gl_texture;
		}
		glDeleteTextures(textures.size(), glt.data());
	}

	void TextureManager::store(Texture& texture) {
		textures.push_back(&texture);
	}
	
	void TextureManager::load(Texture& texture) {
		if (texture.gl_texture != 0) return;
		//cout << "Generating texture from path \"" << texture.path << "\"" << endl;
		glGenTextures(1, &texture.gl_texture);

		if (SDL_Surface* raw = IMG_Load(texture.path.c_str())) {
			if (SDL_Surface* surface = SDL_ConvertSurfaceFormat(raw, SDL_PIXELFORMAT_RGBA32, 0)) {
				//cout << "FORMAT: " << SDL_GetPixelFormatName(surface->format->format) << endl;

				texture.w = surface->w;
				texture.h = surface->h;

				glBindTexture(GL_TEXTURE_2D, texture.gl_texture);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0,
					GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				SDL_FreeSurface(surface);
			}
			SDL_FreeSurface(raw);
		}
	}

	void TextureManager::destroy(Texture& texture) {
		glDeleteTextures(1, &texture.gl_texture);
	}

	Texture::Texture(string path, TextureManager& mgr) 
		: mgr(texture_manager)
		, path(path) 
	{}

	void Texture::draw(Canvas& canvas, int x, int y) {
		load();
		canvas.draw_gl_texture(gl_texture, x, y, w, h);
	}
}
