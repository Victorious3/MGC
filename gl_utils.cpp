#include "stddef.h"

#include "gl_utils.h"

namespace gl_utils {
	void draw_text(int x, int y, TTF_Font *font, SDL_Color color, string text) {
		y = constants::SCR_HEIGHT - y;
		SDL_Surface *textSurface = TTF_RenderUTF8_Blended(font, text.c_str(), color);

		if (textSurface) {

			GLuint texture = 0;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textSurface->w, textSurface->h, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, textSurface->pixels);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texture);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glColor3ub(255, 255, 255);

			glBegin(GL_QUADS);
			{
				glTexCoord2f(0, 1);
				glVertex2i(x, y - textSurface->h);
				glTexCoord2f(0, 0);
				glVertex2i(x, y);
				glTexCoord2f(1, 0);
				glVertex2i(x + textSurface->w, y);
				glTexCoord2f(1, 1);
				glVertex2i(x + textSurface->w, y - textSurface->h);
			}
			glEnd();

			glDisable(GL_BLEND);

			glDisable(GL_TEXTURE_2D);


			glDeleteTextures(1, &texture);
			SDL_FreeSurface(textSurface);
		}
	}
}