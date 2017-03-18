#include <stdafx.h>

#include "Image.h"
//#include "../Texture.h"

namespace UI {
	using namespace render;

	Image::Image(int x, int y, string path, TextureManager& manager)
		: texture(path, manager),
		x(x),
		y(y){}

	Image::~Image()	{
		if (texture.is_loaded()) {
			texture.destroy();
		}
	}

	void Image::update() {

	}

	void Image::render() {
		
	}
}