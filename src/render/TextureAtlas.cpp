#include <stdafx.h>
#include "TextureAtlas.h"

namespace render {

	Sprite& TextureAtlas::add_sprite(string sprite) {
		Uint32 w, h;
		read_img_dim_png(&w, &h, sprite);
		sprites.push_back(SpriteEntry { Sprite(w, h), sprite, 0, 0 });
		return sprites.back().sprite;
	}

	struct Scanline {
		int y, w;
	};

	void TextureAtlas::load() {

		// We have to rebuild the cache
		// First we have to sort the sprites by their surface area
		vector<SpriteEntry*> input_sprites;

		int size_estimate = 0; // The area all sprites would fit into, assuming best fit
		int max_width = 0; // This is the width of our biggest sprite

		int n = 0;
		for (SpriteEntry& sprite : sprites) { 
			size_estimate += sprite.sprite.w * sprite.sprite.h;
			max_width = std::fmaxl(max_width, sprite.sprite.w);
			input_sprites.push_back(&sprite); 	
			n++;
		}

		std::sort(input_sprites.begin(), input_sprites.end(), [](SpriteEntry* a, SpriteEntry* b) { 
			return a->sprite.w * a->sprite.h > b->sprite.w * b->sprite.h;
		});

		// Height and width of our final sprite
		int height = 0, width;
		width = std::fmaxl(std::sqrt(size_estimate), max_width);
		width = std::fminl(width, glvars.max_tex_size);
		if (width < max_width)
			throw RUNTIME_ERROR("Can't stitch texture, MAX_TEX_SIZE too small");

		list<Scanline> upper_edge = { Scanline { 0, width } };

		for (SpriteEntry* sprite : input_sprites) {
			auto it = upper_edge.begin();
			int x = 0;

			{ // We want to find the edge with the smallest y coordinate that fits
				int x2 = 0;
				auto it2 = it;
				do {
					// Could it fit in the first place?
					if (x2 + sprite->sprite.w <= width) {
						if (it2->y < it->y) {
							it = it2;
							x = x2;
						}
					} else break; // Since they are ordered the next one wont fit either
					x2 += it2->w;
				} while (++it2 != upper_edge.end());

				// We found a line to place the texture on
				sprite->x = x;
				sprite->y = it->y;
				height = std::fmaxl(sprite->y + sprite->sprite.h, height);
			}

			// Create a new Scanline for the sprite
			Scanline line { it->y + sprite->sprite.h, sprite->sprite.w };

			{ // Try to combine segments
				auto it2 = upper_edge.insert(it, line);
				if (it2 != upper_edge.begin()) {
					auto it3 = it2; it3--;
					if (it3->y == it2->y) {
						it2->w += it3->w;
						upper_edge.erase(it3);
					}
				}
			}
			
			// Now we have to update the segments ahead
			int x2 = x;
			while (it != upper_edge.end()) {
				int diff = (x + line.w) - (x2 + it->w);
				x2 += it->w;

				// When the segment is entirely covered by the new line we can remove it
				if (diff >= 0) {
					auto cur = it; it++;
					upper_edge.erase(cur);
					if (diff == 0) break; // If the difference is 0 we don't have to split next time
				} else {
					// Otherwise we have to split the segment
					*it = Scanline { it->y, -diff };
					break;
				}
			}

			/*cout << "Scanline size: " << upper_edge.size() << endl;
			for (Scanline& sl : upper_edge) {
				cout << "w: " << sl.w << " y:" << sl.y << endl;
			}
			cout << endl;*/
		}

		// Render our texture
		GLuint gl_texture = allocate_texture(width, height);
		GLuint fbo = create_framebuffer(gl_texture);
		GLuint curr_fbo = bind_framebuffer(fbo);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glViewport(0, 0, width, height);
		gluOrtho2D(0, width, height, 0);
		glMatrixMode(GL_MODELVIEW);
		
		for (SpriteEntry* entry : input_sprites) {
			int w, h;
			GLuint texture = create_texture(entry->path, (Uint*) &w, (Uint*) &h);
			draw_gl_texture(texture, entry->x, entry->y + h, w, -h);
			Sprite& sprite = entry->sprite;

			// Calculate UVs
			sprite.vmin = entry->x / (float) width;
			sprite.umin = entry->y / (float) height;
			sprite.vmax = (entry->x + w) / (float) width;
			sprite.umax = (entry->y + h) / (float) height;

			/*cout << "Sprite: " << entry->path << endl;
			cout << "width: " << w << " height: " << h << endl;
			cout << "x: " << entry->x << " y: " << entry->y << endl;
			cout << "vmin " << sprite.vmin << endl;
			cout << "umin " << sprite.umin << endl;
			cout << "vmax " << sprite.vmax << endl;
			cout << "umax " << sprite.umax << endl;*/
		}
		
		// Write to output
		Uint8* pixel_buffer = new Uint8[width * height * 4];
		
		glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixel_buffer);

		SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormatFrom(pixel_buffer, width, height, 8, width * 4, SDL_PIXELFORMAT_RGBA32);
		SDL_SaveBMP(surface, cache_file.c_str());

		SDL_FreeSurface(surface);
		delete[] pixel_buffer;

		bind_framebuffer(curr_fbo);
		glDeleteFramebuffers(1, &fbo); // Throw away framebuffer

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);

	}

	void TextureAtlas::destroy() {
		glDeleteTextures(1, &texture.gl_texture);
		refresh_cache = true;
	}

}