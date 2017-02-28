#pragma once

#include "stddef.h"

#include "graphics.h"

namespace mgc {
	class Canvas;

	class Sprite {
	public:
		Sprite();
		Sprite(string path);
		~Sprite();

		void load_image(string path);
		void draw(Canvas* canvas, int x, int y);
		SDL_Surface* get_surface() const;
		Texture get_texture(const Canvas* canvas);
		
	private:
		string image_path;
		SDL_Surface* image;
		Texture texture;

		void create_texture(const Canvas* canvas);
	};
}