#include "stddef.h"

#include "canvas.h"

#include "sprite.h"

#include "mgc.h"

SDL_Surface* mgc::Canvas::draw_text_surface(TTF_Font* font, string text) const {
	return TTF_RenderUTF8_Blended(font, text.c_str(), SDL_Color {255, 255, 255, 255});
}

GLuint mgc::Canvas::create_texture(SDL_Surface* surface) const {
	GLuint texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return texture;
}

void mgc::Canvas::draw_texture(int x, int y, int w, int h, GLuint texture, SDL_Color color) const {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4ub(color.r, color.g, color.b, color.a);

	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0);
		glVertex2i(x, y);
		glTexCoord2f(1, 0);
		glVertex2i(x + w, y);
		glTexCoord2f(1, 1);
		glVertex2i(x + w, y + h);
		glTexCoord2f(0, 1);
		glVertex2i(x, y + h);
	}
	glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

void mgc::Canvas::draw_surface(int x, int y, SDL_Surface* surface, SDL_Color color) const {
	if (!surface) return;

	auto texture = create_texture(surface);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	draw_texture(x, y, surface->w, surface->h, texture, color);

	glDeleteTextures(1, &texture);
}

void mgc::Canvas::draw_text(int x, int y, TTF_Font* font, SDL_Color color, string text) const {
	auto text_surface = draw_text_surface(font, text);
	draw_surface(x, y, text_surface, color);
	SDL_FreeSurface(text_surface);
}

void mgc::Canvas::draw_sprite(Sprite* sprite, int x, int y) {
	auto texture = sprite->get_texture(this);
	draw_texture(x, y, texture.w, texture.h, texture.texture, SDL_Color{ 255, 255, 255, 255, });
}

