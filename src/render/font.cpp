#include <stdafx.h>
#include <pendian.h>

#include "../render/font.h"
#include "../filesystem.h"

namespace mgc {

	Font::Font(string file)
		: texture(render::add_texture(file + ".png")) {
		ifstream data(file + ".dat", std::ios_base::binary);
		if (!data) throw RUNTIME_ERROR("Font could not be loaded. File \"" + file + ".dat\"");

		data.read((char*)&map_width, sizeof(Uint32)); map_width = le32toh(map_width);
		data.read((char*)&map_height, sizeof(Uint32)); map_height = le32toh(map_height);
		data.read((char*)&cell_width, sizeof(Uint32)); cell_width = le32toh(cell_width);
		data.read((char*)&cell_height, sizeof(Uint32)); cell_height = le32toh(cell_height);

		data.read(&start_char, 1);
		data.read(reinterpret_cast<char*>(char_sizes), 256);
	}

	Uint Font::char_width(const char& c) const {
		return char_sizes[c];
	}

	Uint Font::add_char(render::VertexBuffer& vb, char c, int x, int y, render::Color color) const {
		texture.load();
		vb.set_texture(texture);
		return add_char_internal(vb, c, x, y, color);
	}

	Uint Font::add_char_internal(render::VertexBuffer& vb, char c, int x, int y, render::Color color) const {
		Uint w = char_width(c);

		Uint32 t = cell_width * (c - start_char);
		float umin = (float)(t % map_width) / map_width;
		float vmin = (float)(t / map_width) * cell_height / map_height;
		float umax = umin + (float)w / map_width;
		float vmax = vmin + (float)cell_height / map_height;

		vb.sprite(x, y, render::Sprite { w, cell_height, umin, vmin, umax, vmax }, color);

		return w;
	}


	Uint Font::add_string(render::VertexBuffer& vb, string s, int x, int y, render::Color color) const {
		Uint w = 0;
		texture.load();
		vb.set_texture(texture);

		int line = 0;
		for (char c : s) {
			if (c == '\n') {
				line++;
				w = 0;
			} else {
				w += add_char_internal(vb, c, x + w, y + line * cell_height, color);
			}
		}
		return w;
	}
}
