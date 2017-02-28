#pragma once

#include "stddef.h"

namespace mgc {
	class Canvas;

	class Sprite {
	public:
		Sprite();
		Sprite(string path);
		~Sprite();

		void load_image(string path);
		void draw(const Canvas* canvas, int x, int y) const;
		SDL_Surface* get_surface() const;
		
	private:
		string image_path;
		SDL_Surface* image;
	};
}