#include "stddef.h"

#include "sprite.h"

#include "canvas.h"

#include "graphics.h"

mgc::Sprite::Sprite() : texture{0, 0, 0} {
}

mgc::Sprite::Sprite(string path) : texture{ 0, 0, 0 } {
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

void mgc::Sprite::draw(Canvas* canvas, int x, int y) {
	if (texture.texture == 0)
		create_texture(canvas);
	canvas->draw_sprite(this, x, y);
}

void mgc::Sprite::create_texture(const Canvas* canvas) {
	texture.w = image->w;
	texture.h = image->h;
	texture.texture = canvas->create_texture(get_surface());
}

SDL_Surface* mgc::Sprite::get_surface() const {
	return image;
}

mgc::Texture mgc::Sprite::get_texture(const Canvas* canvas) {
	if (texture.texture == 0) {
		create_texture(canvas);
	}
	return texture;
}