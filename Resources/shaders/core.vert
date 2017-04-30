#version 330 core

in ivec2 vertex;
in vec2 texture;
in vec4 color;

out vec2 frag_texture;
out vec4 frag_color;

uniform mat4 projection; 

void main() {
	gl_Position = projection * vec4(vertex, 0.0, 1.0);

	frag_texture = texture;
	frag_color = color;
}