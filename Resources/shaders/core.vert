#version 330 core

in ivec2 position;
in vec2 texture;
in vec4 color;

out vec2 frag_texture;
out vec4 frag_color;

uniform mat4 projection; 

void main() {
	const mat4 test_matrix = mat4(
		vec4( 0.003125,  0.000000,  0.000000, 0.000000), 
		vec4( 0.000000, -0.005556,  0.000000, 0.000000), 
		vec4( 0.000000,  0.000000, -1.000000, 0.000000), 
		vec4(-1.000000,  1.000000,  0.000000, 1.000000));

	gl_Position = projection * vec4(position, 0.0, 1.0);

	frag_texture = texture;
	frag_color = color;
}