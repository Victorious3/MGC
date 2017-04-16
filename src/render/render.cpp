#include <stdafx.h>

#include "render.h"
#include "../mgc.h"
#include "../log.h"

namespace render {

	GLVars glvars;
	
	void init_glvars() {
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glvars.max_tex_size);
	}

	GLuint allocate_texture(Uint w, Uint h, const void* pBuffer) {
		GLuint tex;
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pBuffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		return tex;
	}

	GLuint create_framebuffer(GLuint texture_attachment) {
		GLuint fbo; glGenFramebuffers(1, &fbo);
		GLint current_fbo = bind_framebuffer(fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_attachment, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			mgc::log::error << "Framebuffer creation failed" << endl;
		glBindFramebuffer(GL_FRAMEBUFFER, current_fbo);
		return fbo;
	}

	GLuint bind_framebuffer(GLuint fbo) {
		GLint current_fbo;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &current_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		return current_fbo;
	}

	void draw_sprite(const Sprite& sprite, int x, int y, Color color) {
		draw_gl_texture(sprite.gl_texture, x, y, sprite.w, sprite.h, sprite.umin, sprite.vmin, sprite.umax, sprite.vmax, color);
	}

	// Ensures that the texture is properly loaded
	void draw_texture(Texture& texture, int x, int y, Color color) {
		texture.load();
		draw_gl_texture(texture.gl_texture, x, y, texture.w, texture.h, texture.umin, texture.vmin, texture.umax, texture.vmax, color);
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

