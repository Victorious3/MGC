#include <stdafx.h>

#include "render.h"
#include "../mgc.h"
#include "../log.h"
#include "VertexBuffer.h"

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

	void gl_object_label(const string& str, GLenum identifier, GLuint name) {
#ifndef NDEBUG
		if (GLEW_KHR_debug) {
			glObjectLabel(identifier, name, str.length(), str.c_str());
		}
#endif
	}

	GLuint create_framebuffer(GLuint texture_attachment) {
		GLuint fbo; glGenFramebuffers(1, &fbo);
		GLint current_fbo = bind_framebuffer(fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_attachment, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			mgc::log::error("Framebuffer creation failed");
		glBindFramebuffer(GL_FRAMEBUFFER, current_fbo);
		return fbo;
	}

	GLuint bind_framebuffer(GLuint fbo) {
		GLint current_fbo;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &current_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		return current_fbo;
	}
}

