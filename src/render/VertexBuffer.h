#pragma once
#include <stdafx.h>

#include "render.h"

namespace render {
	class VertexBuffer {
	public:
		void init();
		void destroy();

		void vertex(int x, int y);
		void uv(float u, float v);
		void texture(GLuint texture);
		void color(Color color);
		
		void sprite(const Sprite& sprite);

		template<GLenum primitive>
		void draw() {
			// TODO
		}

		void clear();

	private:
		GLuint gl_vao;
	};
}

