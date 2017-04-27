#pragma once

#include <stdafx.h>

namespace render {

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
		void compile(std::initializer_list<Shader> shaders);

		operator GLuint() {
			return gl_program;
		}

		void operator()() {
			glUseProgram(gl_program);
		}

		GLuint gl_program;
	};

	extern ShaderProgram core_shader;

	void load_shaders();
	void destroy_shaders();
}
