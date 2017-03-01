#include "stddef.h"

#include "canvas.h"
#include "texture.h"
#include "mgc.h"

namespace mgc {

	constexpr Color color(Uint8 r, Uint8 g, Uint8 b) {
		return Color { r, g, b, 255 };
	}

	constexpr Color color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
		return Color { r, g, b, a };
	}

	constexpr Color color(Uint32 rbg) {
		return Color { (Uint8)(rbg >> 16), (Uint8)(rbg >> 8), (Uint8)rbg, 255};
	}

	constexpr Color color_a(Uint32 rbga) {
		return Color { (Uint8)(rbga >> 24), (Uint8)(rbga >> 16), (Uint8)(rbga >> 8) };
	}

	SDL_Surface* Canvas::draw_text_surface(TTF_Font& font, string text) const {
		return TTF_RenderUTF8_Blended(&font, text.c_str(), colors::WHITE);
	}

	void Canvas::draw_texture(int x, int y, int w, int h, Uint texture, Color color) const {
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

	void Canvas::draw_surface(int x, int y, SDL_Surface& surface, Color color) const {

		Uint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface.w, surface.h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, surface.pixels);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		draw_texture(x, y, surface.w, surface.h, texture, color);

		glDeleteTextures(1, &texture);
	}

	void Canvas::draw_text(int x, int y, TTF_Font& font, Color color, string text) const {
		auto* text_surface = draw_text_surface(font, text);
		draw_surface(x, y, *text_surface, color);
		SDL_FreeSurface(text_surface);
	}
}

