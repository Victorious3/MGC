#pragma once
#include <stdafx.h>

#include "texture.h"
#include "render.h";
#include "VertexBuffer.h"

namespace mgc {

	class Font final {
	public:
		Font(string file);

		Uint char_width(const char& c) const;
		Uint add_char(render::VertexBuffer& vb, char c, int x, int y, render::Color color = render::colors::WHITE) const;
		Uint add_string(render::VertexBuffer& vb, string s, int x, int y, render::Color color = render::colors::WHITE) const;
		
	private:
		// Sprite
		render::Texture& texture;

		Uint32 map_width;
		Uint32 map_height;
		Uint32 cell_width;
		Uint32 cell_height;

		char start_char;

		Uint8 char_sizes[256];

		Uint add_char_internal(render::VertexBuffer& vb, char c, int x, int y, render::Color color = render::colors::WHITE) const;
	};
}