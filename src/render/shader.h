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
		ShaderProgram() {};
		ShaderProgram(std::initializer_list<Shader> shaders);
	
		void compile();

		operator GLuint() {
			return gl_program;extern ShaderProgram core_shader;
		}

		void operator()() {
			glUseProgram(gl_program);
		}

		vector<Shader> shaders;
		GLuint gl_program;
	};

	extern ShaderProgram core_shader;

	void load_shaders();
	void destroy_shaders();
}
