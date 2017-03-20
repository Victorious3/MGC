#include <stdafx.h>

#include "texture.h"
#include "render.h"

namespace render {

	TextureManager texture_manager;

	void TextureManager::load_all() {
		for (auto it : textures) {
			load(it.second);
		}
	}

	void TextureManager::reload() {
		destroy_all();
		load_all();
	}

	void TextureManager::destroy_all() {
		vector<GLuint> glt(textures.size());
		for (auto it : textures) {
			glt.push_back(it.second.gl_texture);
		}
		glDeleteTextures(textures.size(), glt.data());
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
		texture.gl_texture = 0;
	}

	Texture& TextureManager::get_texture(string path) {
		auto tex = textures.find(path);
		if (tex != textures.end()) {
			return tex->second;
		}
	}
}
