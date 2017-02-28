#include "stddef.h"

#include "sprite.h"

#include "canvas.h"

mgc::Sprite::Sprite() {
}

mgc::Sprite::Sprite(string path) {
	load_image(path);
}

mgc::Sprite::~Sprite() {
	SDL_FreeSurface(image);
}

void mgc::Sprite::load_image(string path) {
	image_path = path;

	image = IMG_Load(path.c_str());
	if (image == nullptr) {
		SDL_LogError(SDL_LOG_CATEGORY_VIDEO, ("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError()));
	}
}

void mgc::Sprite::draw(const Canvas* canvas, int x, int y) const {
	canvas->draw_sprite(this, x, y);
}

SDL_Surface* mgc::Sprite::get_surface() const {
	return image;
}