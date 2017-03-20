#include <stdafx.h>

#include "Image.h"
#include "../render.h"

namespace UI {
	using namespace render;

	Image::Image(int x, int y, string path, TextureManager& manager)
		: texture(manager.get_texture(path)),
		x(x),
		y(y){}

	void Image::update() {

	}

	void Image::render() {
		texture.load();
		render::draw_texture(texture, x, y, colors::WHITE);
	}
}