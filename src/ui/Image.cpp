#include <stdafx.h>

#include "Image.h"
//#include "../Texture.h"

namespace UI {
	using namespace render;

	Image::Image(int x, int y, string path, TextureManager& manager)
		: texture(manager.get_texture(path)),
		x(x),
		y(y){}

	void Image::update() {

	}

	void Image::render() {
		
	}
}