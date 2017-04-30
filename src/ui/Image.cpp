#include <stdafx.h>

#include "Image.h"

using namespace render;

namespace UI {
	Image::Image(int x, int y, string path, TextureManager& manager)
		: texture(manager.add_texture(path)),
		x(x),
		y(y){}

	void Image::update() {

	}

	void Image::render() {
		// TODO deprecated
		//draw_texture(texture, x, y, colors::WHITE);
	}
}