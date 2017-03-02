#include "stddef.h"

#include <pendian.h>

#include "font.h"
#include "filesystem.h"

namespace mgc {

	static void read_Uint64(Uint32* out, ifstream& data) {
		char buffer[4];
		data.read(buffer, 4);
		std::memcpy(out, buffer, 4);
		*out = le32toh(*out);
	}

	Font::Font(string file) 
		: texture(file + ".png")
	{
		ifstream data(file + ".dat", std::ios_base::binary);
		if (!data) throw runtime_error("Font could not be loaded. File \"" + file + ".dat\"");
		
		read_Uint64(&map_width, data);
		read_Uint64(&map_height, data);
		read_Uint64(&cell_width, data);
		read_Uint64(&cell_height, data);
		
		data.read(&start_char, 1);
		data.read((char*) char_sizes, 256);
 	}

	Uint Font::char_width(char c) {
		return char_sizes[c];
	}

	Uint Font::draw_char_internal(Canvas& canvas, char c, int x, int y, Color color) {
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

	Uint Font::draw_char(Canvas& canvas, char c, int x, int y, Color color) {
		Uint w;

		texture.load();
		canvas.enable_gl_texture();
		glBindTexture(GL_TEXTURE_2D, texture.gl_texture);
		glBegin(GL_QUADS);
		w = draw_char_internal(canvas, c, x, y, color);
		glEnd();
		canvas.disable_gl_texture();

		return w;
	}

	Uint Font::draw_string(Canvas& canvas, string s, int x, int y, Color color) {
		Uint w = 0;
		texture.load();
		canvas.enable_gl_texture();
		glBindTexture(GL_TEXTURE_2D, texture.gl_texture);
		glColor4ub(color.r, color.g, color.b, color.a);
		glBegin(GL_QUADS);

		for (char c : s) {
			w += draw_char_internal(canvas, c, x + w, y, color);
		}

		glEnd();
		canvas.disable_gl_texture();
		return w;
	}
}