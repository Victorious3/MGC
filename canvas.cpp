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

	void Canvas::draw_gl_texture(GLuint texture, int x, int y, int w, int h, Color color) const {
		draw_gl_texture(texture, x, y, w, h, 0, 0, 1, 1, color);
	}

	void Canvas::draw_gl_texture(GLuint texture, int x, int y, int w, int h, float umin, float vmin, float umax, float vmax, Color color) const {
		enable_gl_texture();
		glBindTexture(GL_TEXTURE_2D, texture);
		glColor4ub(color.r, color.g, color.b, color.a);

		glBegin(GL_QUADS);
		{
			glTexCoord2f(umin, vmin);
			glVertex2i(x, y);
			glTexCoord2f(umax, vmin);
			glVertex2i(x + w, y);
			glTexCoord2f(umax, vmax);
			glVertex2i(x + w, y + h);
			glTexCoord2f(umin, vmax);
			glVertex2i(x, y + h);
		}
		glEnd();

		disable_gl_texture();
	}
}

