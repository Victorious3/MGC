#include <stdafx.h>
#include <pendian.h>

#include "../render/font.h"
#include "../filesystem.h"

namespace mgc {

	Font::Font(string file) 
		: texture(render::add_texture(file + ".png"))
	{
		ifstream data(file + ".dat", std::ios_base::binary);
		if (!data) throw RUNTIME_ERROR("Font could not be loaded. File \"" + file + ".dat\"");
		
		data.read((char*)&map_width,   sizeof(Uint32)); map_width   = le32toh(map_width);
		data.read((char*)&map_height,  sizeof(Uint32)); map_height  = le32toh(map_height);
		data.read((char*)&cell_width,  sizeof(Uint32)); cell_width  = le32toh(cell_width);
		data.read((char*)&cell_height, sizeof(Uint32)); cell_height = le32toh(cell_height);
		
		data.read(&start_char, 1);
		data.read(reinterpret_cast<char*>(char_sizes), 256);
 	}

	Uint Font::char_width(const char& c) const {
		return char_sizes[c];
	}

	Uint Font::draw_char_internal(char c, int x, int y, render::Color color) const {
		Uint w = char_width(c);

		Uint32 t = cell_width * (c - start_char);
		float umin = (float)(t % map_width) / map_width;
		float vmin = (float)(t / map_width) * cell_height / map_height;
		float umax = umin + (float)w / map_width;
		float vmax = vmin + (float)cell_height / map_height;
		// cout << umin << " " << vmin << " " << umin << " " << umax << endl;

		glTexCoord2f(umin, vmin);
		glVertex2i(x, y);
		glTexCoord2f(umax, vmin);
		glVertex2i(x + w, y);
		glTexCoord2f(umax, vmax);
		glVertex2i(x + w, y + cell_height);
		glTexCoord2f(umin, vmax);
		glVertex2i(x, y + cell_height);

		return w;
	}

	Uint Font::draw_char(char c, int x, int y, render::Color color) {
		Uint w;

		texture.load();
		render::enable_gl_texture();
		glBindTexture(GL_TEXTURE_2D, texture.gl_texture);
		glBegin(GL_QUADS);
		w = draw_char_internal(c, x, y, color);
		glEnd();
		render::disable_gl_texture();

		return w;
	}

	Uint Font::draw_string(string s, int x, int y, render::Color color) {
		Uint w = 0;
		texture.load();
		render::enable_gl_texture();
		glBindTexture(GL_TEXTURE_2D, texture.gl_texture);
		glColor4ub(color.r, color.g, color.b, color.a);
		glBegin(GL_QUADS);

		int line = 0;
		for (char c : s) {
			if (c == '\n') {
				line++;
				w = 0;
			} else {
				w += draw_char_internal(c, x + w, y + line * cell_height, color);
			}
		}

		glEnd();
		render::disable_gl_texture();
		return w;
	}
}