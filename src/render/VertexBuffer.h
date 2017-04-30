#pragma once
#include <stdafx.h>

#include "render.h"

namespace render {
	class VertexBuffer;

	class VertexPtr {
	public:
		VertexPtr(Uint index, VertexBuffer& buffer) : index(index), buffer(buffer) {};
		
		VertexPtr& uv(float u, float v);
		VertexPtr& uv(glm::vec2 v) {
			return uv(v.x, v.y);
		}
		VertexPtr& color(Color color);
	private:
		const Uint index;
		VertexBuffer& buffer;
	};

	// TODO Find a better name for this
	class VertexBuffer {
	public:
		~VertexBuffer() {
			destroy();
		}

		// Frees all underlying GL objects
		void destroy();

		void set_texture(GLuint texture);
		VertexPtr vertex(int x, int y);
		VertexPtr vertex(glm::ivec2 v) {
			return vertex(v.x, v.y);
		}

		void sprite(int x, int y, const Sprite& sprite, Color color = colors::WHITE);
		void sprite(int x, int y, int w, int h, const Sprite& sprite, Color color = colors::WHITE);

		void rect(int x, int y, int w, int h);

		// Compiles the buffers
		void compile();
		// Clear the cached vertices, frees up RAM, you can still call draw() after this
		void clear();

		// Draws, leaves all buffers untouched. You have to call compile() before it does anything
		void draw(GLenum primitive = GL_TRIANGLES);

		// Draws like immediate mode, clears all buffers
		void draw_imm(GLenum primitive = GL_TRIANGLES) {
			compile();
			draw(primitive); 
			clear();
			destroy_buffers();
		}

	private:
		friend VertexPtr;

		void destroy_buffers();

		GLuint gl_vao = 0;

		// Cached geometry per texture

		struct ClientData {
			ClientData(GLuint texture) : gl_texture(texture) {};

			const GLuint gl_texture;

			vector<glm::ivec2> vertex;
			vector<glm::vec2> texture;
			vector<Color> color;
		};

		vector<ClientData> data { ClientData { 0 } };
		ClientData* current_data = &data.front();

		struct VBO {
			GLuint gl_texture;

			// Size of the buffers
			Uint size;

			GLuint vertex;
			GLuint texture;
			GLuint color;
		};

		vector<VBO> vboa;
	};
}

