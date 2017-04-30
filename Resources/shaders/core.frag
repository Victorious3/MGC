#version 330 core

uniform sampler2D tex;

in vec2 frag_texture;
in vec4 frag_color;

out vec4 color;

void main() {
	//color = frag_color * texture(tex, frag_texture);
	color = vec4(0.5, 0.2, 0.1, 1);
} 