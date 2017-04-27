#include <stdafx.h>

#include "shader.h"
#include "../filesystem.h"
#include "../log.h"

namespace render {

	ShaderProgram core_shader;

	void load_shaders() {
		core_shader.compile({ Shader("Resources/shaders/core.frag"), Shader("Resources/shaders/core.vert") });
	}

	void destroy_shaders() {
		glDeleteProgram(core_shader);
	}

	static GLenum get_shader_type(const string& file_name) {
		if (string_ends_with(file_name, ".frag")) return GL_FRAGMENT_SHADER;
		else if (string_ends_with(file_name, ".vert")) return GL_VERTEX_SHADER;
		throw RUNTIME_ERROR("Invalid file type extension for shader \"" + file_name + "\"");
	}

	Shader::Shader(const string file_name) : file_name(file_name), type(get_shader_type(file_name)) {
		compile();
	}

	Shader::~Shader() {
		glDeleteShader(gl_shader);
	}

	void Shader::compile() {
		ifstream in { file_name };
		if (!in.is_open()) 
			throw RUNTIME_ERROR("Can't open shader file \"" + file_name + "\"");
		string source_str;
		std::string line = "";
		while (!in.eof()) {
			std::getline(in, line);
			source_str += (line + "\n");
		}
		const char* source = source_str.c_str();

		GLint compile_status = false;
		int log_length = 0;

		gl_shader = glCreateShader(type);
		glShaderSource(gl_shader, 1, &source, nullptr);
		glCompileShader(gl_shader);

		glGetShaderiv(gl_shader, GL_COMPILE_STATUS, &compile_status);
		glGetShaderiv(gl_shader, GL_INFO_LOG_LENGTH, &log_length);

		if (log_length > 0) {
			char* shader_log = new char[log_length + 1];
			shader_log[log_length] = '\0'; // 0 terminate!
			glGetShaderInfoLog(gl_shader, log_length, nullptr, shader_log);
			mgc::log::info << shader_log << endl;
			delete[] shader_log;
		}
		if (!compile_status) {
			throw new RUNTIME_ERROR("Shader compilation failed for shader \"" + file_name + "\", see program log");
		}
	}

	void ShaderProgram::compile(std::initializer_list<Shader> shaders) {
		if (gl_program) glDeleteProgram(gl_program);
		gl_program = glCreateProgram();

		for (auto& shader : shaders) {
			glAttachShader(gl_program, shader.gl_shader);
		}
		glLinkProgram(gl_program);

		GLint link_status = false;
		int log_length = 0;

		glGetProgramiv(gl_program, GL_LINK_STATUS, &link_status);
		glGetProgramiv(gl_program, GL_INFO_LOG_LENGTH, &log_length);

		if (log_length > 0) {
			char* program_log = new char[log_length + 1];
			program_log[log_length] = '\0'; // 0 terminate!
			glGetProgramInfoLog(gl_program, log_length, nullptr, program_log);
			mgc::log::info << program_log << endl;
			delete[] program_log;
		}
		if (!link_status) {
			throw new RUNTIME_ERROR("Program linking failed");
		}
	}
}
