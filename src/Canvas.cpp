#include <stdafx.h>

#include "canvas.h"
#include "texture.h"
#include "mgc.h"

namespace mgc {

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

