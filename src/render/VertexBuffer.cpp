#include <stdafx.h>
#include "VertexBuffer.h"

#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

namespace render {

	VertexPtr& VertexPtr::uv(float u, float v) {
		buffer.current_data->texture[index] = glm::vec2(u, v);
		return *this;
	}

	VertexPtr& VertexPtr::color(Color color) {
		buffer.current_data->color[index] = color;
		return *this;
	}

	void VertexBuffer::set_texture(GLuint texture) {
		if (current_data->gl_texture != texture) {
			auto it = std::find_if(data.begin(), data.end(), [texture](ClientData& data) { return data.gl_texture == texture; });
			if (it != data.end()) {
				current_data = &(*it);
			} else {
				data.push_back(ClientData { texture });
				current_data = &data.back();
			}
		}
	}

	VertexPtr VertexBuffer::vertex(int x, int y) {
		current_data->vertex.push_back(glm::ivec2(x, y));
		current_data->texture.push_back(glm::vec2(0, 0));
		current_data->color.push_back(colors::WHITE);
		return VertexPtr(current_data->vertex.size() - 1, *this);
	}

	void VertexBuffer::sprite(int x, int y, const Sprite& sprite, Color color) {
		VertexBuffer::sprite(x, y, sprite.w, sprite.h, sprite, color);
	}

	void VertexBuffer::sprite(int x, int y, int w, int h, const Sprite& sprite, Color color) {
		// Two triangles, quads are dead, long live quads!
		vertex(x    , y    ).uv(sprite.umin, sprite.vmin).color(color);
		vertex(x + w, y + h).uv(sprite.umax, sprite.vmax).color(color);
		vertex(x    , y + h).uv(sprite.umin, sprite.vmax).color(color);

		vertex(x + w, y    ).uv(sprite.umax, sprite.vmin).color(color);
		vertex(x + w, y + h).uv(sprite.umax, sprite.vmax).color(color);
		vertex(x    , y    ).uv(sprite.umin, sprite.vmin).color(color);
	}

	void VertexBuffer::rect(int x, int y, int w, int h) {
		vertex(x, y);
		vertex(x + w, y + h);
		vertex(x, y + h);

		vertex(x + w, y);
		vertex(x + w, y + h);
		vertex(x, y);
	}

	void VertexBuffer::compile() {
		if (!gl_vao) {
			glGenVertexArrays(1, &gl_vao);
			glBindVertexArray(gl_vao);

			glEnableVertexAttribArray(Attrib::VERTEX);
			glEnableVertexAttribArray(Attrib::TEXTURE);
			glEnableVertexAttribArray(Attrib::COLOR);
		} else {
			glBindVertexArray(gl_vao);
		}

		if (vboa.size() > 0) destroy_buffers();

		for (ClientData& cl_data : data) {
			if (cl_data.vertex.size() > 0) {
				GLuint buffers[3];

				glGenBuffers(3, buffers);
				glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
				glBufferData(GL_ARRAY_BUFFER, cl_data.vertex.size() * sizeof(glm::ivec2), cl_data.vertex.data(), GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
				glBufferData(GL_ARRAY_BUFFER, cl_data.texture.size() * sizeof(glm::vec2), cl_data.texture.data(), GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
				glBufferData(GL_ARRAY_BUFFER, cl_data.color.size() * sizeof(Color), cl_data.color.data(), GL_STATIC_DRAW);

				vboa.push_back(VBO { cl_data.gl_texture, cl_data.vertex.size(), buffers[0], buffers[1], buffers[2] });
			}
		}

		glBindVertexArray(0);
	}

	void VertexBuffer::destroy_buffers() {
		for (VBO& vbo : vboa) {
			glDeleteBuffers(1, &vbo.vertex);
			glDeleteBuffers(1, &vbo.texture);
			glDeleteBuffers(1, &vbo.color);
		}
		vboa.clear();
		vboa.shrink_to_fit();
	}

	void VertexBuffer::destroy() {
		destroy_buffers();

		glDeleteVertexArrays(1, &gl_vao);
	}

	void VertexBuffer::clear() {
		data.clear();
		data.shrink_to_fit();
		data.emplace_back(0);
		current_data = &data.back();
	}

	void VertexBuffer::draw(GLenum primitive) {
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(gl_vao);

		for (VBO& vbo : vboa) {
			glBindTexture(GL_TEXTURE_2D, vbo.gl_texture);

			glBindBuffer(GL_ARRAY_BUFFER, vbo.vertex);
			glVertexAttribIPointer(Attrib::VERTEX, 2, GL_INT, sizeof(glm::ivec2), 0);
			glBindBuffer(GL_ARRAY_BUFFER, vbo.texture);
			glVertexAttribPointer(Attrib::TEXTURE, 2, GL_FLOAT, false, sizeof(glm::vec2), 0);
			glBindBuffer(GL_ARRAY_BUFFER, vbo.color);
			glVertexAttribPointer(Attrib::COLOR, 4, GL_UNSIGNED_BYTE, true, sizeof(Color), 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glDrawArrays(primitive, 0, vbo.size);
		}

		glBindVertexArray(0);
	}
}
