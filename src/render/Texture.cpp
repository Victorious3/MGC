#include <stdafx.h>
#include "texture.h"

#include <pendian.h>

#include "render.h"
#include "../filesystem.h"

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
		texture.gl_texture = create_texture(texture.path, &texture.w, &texture.h);
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

	GLuint create_texture(const string& path, Uint* w, Uint* h) {
		GLuint texture = 0;
		//cout << "Generating texture from path \"" << texture.path << "\"" << endl;
		if (SDL_Surface* raw = IMG_Load(path.c_str())) {
			if (SDL_Surface* surface = SDL_ConvertSurfaceFormat(raw, SDL_PIXELFORMAT_RGBA32, 0)) {
				//cout << "FORMAT: " << SDL_GetPixelFormatName(surface->format->format) << endl;
				*w = surface->w;
				*h = surface->h;
				texture = allocate_texture(surface->w, surface->h, surface->pixels);

				SDL_FreeSurface(surface);
			} else throw RUNTIME_ERROR("Couldn't convert image " + path + " to internal gl representation");
			SDL_FreeSurface(raw);
		} else throw RUNTIME_ERROR("Couldn't load PNG " + path);

		return texture;
	}

	void read_img_dim_png(Uint32* w, Uint32* h, const string& file) {
		ifstream in(file, std::ios_base::binary);
		if (!in.good()) 
			throw RUNTIME_ERROR("Couldn't get read image file " + file);

		// Validade that we have a correct png file
		// We have to do this because anyone could change the folder structure, resulting in very nasty errors

		char buff[8];
		static const char png_magic[] { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
		static const char png_ihdr[] { 0x49, 0x48, 0x44, 0x52 };

		in.read(buff, 8); // PNG magic number
		if (std::strncmp(buff, png_magic, 8) != 0)
			throw RUNTIME_ERROR("Not a valid png file! (Magic Number): " + file);
		in.ignore(4); // Chunk length, we dont care about this, supposedly 13
		in.read(buff, 4); // What matters is that we have the IHDR chunk here
		if (std::strncmp(buff, png_ihdr, 4) != 0)
			throw RUNTIME_ERROR("Not a valid png file! (IHDR): " + file);
		
		in.read((char*)w, sizeof(Uint32)); *w = be32toh(*w);
		in.read((char*)h, sizeof(Uint32)); *h = be32toh(*h);
	}
}
