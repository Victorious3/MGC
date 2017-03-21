#include <stdafx.h>
#include "texture.h"

#include <pendian.h>

#include "render.h"
#include "filesystem.h"

namespace render {

	TextureManager texture_manager;

	void TextureManager::load_all() {
		for (auto it : textures) {
			load(it.second);
		}
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
		if (SDL_Surface* raw = IMG_Load(texture.path.c_str())) {
			if (SDL_Surface* surface = SDL_ConvertSurfaceFormat(raw, SDL_PIXELFORMAT_RGBA32, 0)) {
				//cout << "FORMAT: " << SDL_GetPixelFormatName(surface->format->format) << endl;
				texture.w = surface->w;
				texture.h = surface->h;
				texture.gl_texture = allocate_texture(surface->w, surface->h, surface->pixels);

				SDL_FreeSurface(surface);
			} else throw RUNTIME_ERROR("Couldn't convert image " + texture.path + " to internal gl representation");
			SDL_FreeSurface(raw);
		} else throw RUNTIME_ERROR("Couldn't load PNG " + texture.path);
	}

	void TextureManager::destroy(Texture& texture) {
		glDeleteTextures(1, &texture.gl_texture);
		texture.gl_texture = 0;
	}

	Texture& TextureManager::add_texture(string path) {
		auto tex = textures.find(path);
		if (tex != textures.end()) {
			return tex->second;
		}
		auto it = textures.emplace(path, Texture(path, *this));
		return it.first->second;
	}

	Sprite& TextureAtlas::add_sprite(string sprite) {
		Uint32 w, h;
		read_img_dim_png(&w, &h, sprite);
		sprites.push_back(Sprite(w, h));
		return sprites.back();
	}

	void TextureAtlas::load() {
		if (!refresh_cache && !cache_file.empty()) {
			// First we try to read the cache
			// The cache file is a simple binary file that stores the computed positions of the sprites
			// and the size of the cache file
			ifstream cachef(cache_file + ".dat", std::ios_base::binary);
			if (cachef.good()) {
				// TODO

				return;
			}
		}

		// We have to rebuild the cache
	}

	void TextureAtlas::destroy() {
		glDeleteTextures(1, &texture.gl_texture);
		refresh_cache = true;
	}
	
	void read_img_dim_png(Uint32* w, Uint32* h, const string& file) {
		ifstream in(file);
		if (!in.good()) 
			throw RUNTIME_ERROR("Couldn't get read image file " + file);

		// Validade that we have a correct png file
		// We have to do this because anyone could change the folder structure, resulting in very nasty errors

		char buff[8];
		static const char png_magic[] { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
		static const char png_ihdr[] { 0x49, 0x48, 0x44, 0x52 };

		in.readsome(buff, 8); // PNG magic number
		if (!std::strncmp(buff, png_magic, 8))
			throw RUNTIME_ERROR("Not a valid png file! " + file);
		in.seekg(4); // Chunk length, we dont care about this, supposedly 13
		in.readsome(buff, 4); // What matters is that we have the IHDR chunk here
		if (!std::strncmp(buff, png_ihdr, 4))
			throw RUNTIME_ERROR("Not a valid png file! " + file);
		
		in.read((char*)w, sizeof(Uint32)); *w = be32toh(*w);
		in.read((char*)h, sizeof(Uint32)); *h = be32toh(*h);
	}
}
