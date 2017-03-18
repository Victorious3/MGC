#include <stdafx.h>

#include "render.h"
#include "Texture.h"
#include "mgc.h"

namespace render {
	void draw_sprite(const Sprite& sprite, int x, int y, Color color) {
		draw_gl_texture(sprite.gl_texture, x, y, sprite.w, sprite.h, sprite.umin, sprite.vmin, sprite.umax, sprite.vmax, color);
	}

	void draw_gl_texture(GLuint texture, int x, int y, int w, int h, Color color) {
		draw_gl_texture(texture, x, y, w, h, 0, 0, 1, 1, color);
	}

	void draw_gl_texture(GLuint texture, int x, int y, int w, int h, float umin, float vmin, float umax, float vmax, Color color) {
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

