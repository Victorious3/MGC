#include "stddef.h"

#include "canvas.h"

#include "sprite.h"

SDL_Surface* mgc::Canvas::draw_text_surface(TTF_Font* font, string text) const {
	return TTF_RenderUTF8_Blended(font, text.c_str(), SDL_Color {255, 255, 255, 255});
}

void mgc::Canvas::draw_surface(int x, int y, SDL_Surface* surface, SDL_Color color) const {
	if (!surface) return;

	GLuint texture = 0;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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
		glVertex2i(x + surface->w, y);
		glTexCoord2f(1, 1);
		glVertex2i(x + surface->w, y + surface->h);
		glTexCoord2f(0, 1);
		glVertex2i(x, y + surface->h);
	}
	glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	glDeleteTextures(1, &texture);
}

void mgc::Canvas::draw_text(int x, int y, TTF_Font* font, SDL_Color color, string text) const {
	SDL_Surface* text_surface = draw_text_surface(font, text);
	draw_surface(x, y, text_surface, color);
	SDL_FreeSurface(text_surface);
}

void mgc::Canvas::draw_sprite(const Sprite* sprite, int x, int y) const {
	draw_surface(x, y, sprite->get_surface(), SDL_Color{ 255, 255, 255, 255, });
}

