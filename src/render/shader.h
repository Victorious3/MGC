#pragma once

#include <stdafx.h>

namespace render {

	enum Attrib : GLuint {
		VERTEX = 0,
		TEXTURE = 1,
		COLOR = 2
	};

	class Shader {
	public:
		Shader(const string file_name);
		~Shader();

		void compile();

		const GLenum type;
		GLuint gl_shader;
	
		const string file_name;
	};

	class ShaderProgram {
	public:
		void bind(std::initializer_list<Shader> shaders);
		void link();

		operator GLuint() {
			return gl_program;
		}

		void operator()() {
			glUseProgram(gl_program);
		}

		GLuint gl_program;
	};

	class CoreShader : public ShaderProgram {
	public:
		GLuint projection;
		const GLuint vertex = VERTEX;
		const GLuint texture = TEXTURE;
		const GLuint color = COLOR;
	};

	extern CoreShader core_shader;

	void load_shaders();
	void destroy_shaders();
}
