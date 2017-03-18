#include <stdafx.h>

#include "Image.h"
//#include "../Texture.h"

namespace mgc {
	namespace UI {
		Image::Image(int x, int y, string path, TextureManager& manager, Canvas& canvas)
			: texture(path, manager),
			x(x),
			y(y),
			canvas(canvas) { }

		Image::~Image()	{
			if (texture.is_loaded()) {
				texture.destroy();
			}
		}

		void Image::update() {

		}

		void Image::render() {
			texture.draw(canvas, x, y);
		}
	}
}