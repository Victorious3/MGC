#include <stdafx.h>
#include "TextureAtlas.h"

// Force power of two textures
#define FORCE_POT true
// Toggle writing of the atlas to a temporary directory
#define WRITE_ATLAS_TO_FILE false
#define WRITE_ATLAS_OUTPUT_FILE "Resources/atlas/"

namespace render {

	Sprite& TextureAtlas::add_sprite(string sprite) {
		Uint32 w, h;
		read_img_dim_png(&w, &h, sprite);
		return sprites.insert(SpriteEntry { Sprite(w, h), sprite, 0, 0 }).first->sprite;
	}

	struct Scanline {
		int y, w;
	};

	bool TextureAtlas::SpriteEntry::operator==(const SpriteEntry& e) const {
		return path == e.path;
	}

	size_t TextureAtlas::SpriteEntry::hash::operator()(const SpriteEntry& e) const {
		return std::hash<string>()(e.path);
	}

	// TODO Clean this up a bit (Especially the goto)
	void TextureAtlas::create_bucket(list<const SpriteEntry*>& sprites) {

		int size_estimate = 0; // The area all sprites would fit into, assuming best fit
		int max_size = glvars.max_tex_size * glvars.max_tex_size;

		vector<const SpriteEntry*> stitched_textures; // A list of the textures stitched successfully

		for (auto entry : sprites) {
			size_estimate += entry->sprite.w * entry->sprite.h;
			if (size_estimate > max_size) {
				size_estimate = max_size;
				break;
			}
		}

		// Height and width of our final sprite
		int height = 0, width;
		width = std::sqrt(size_estimate);

#if FORCE_POT
		width = next_power_of_two(width);
#endif

		list<Scanline> upper_edge = { Scanline { 0, width } };

		auto entry = sprites.begin();
		while (entry != sprites.end()) {
			auto sprite_entry = (*entry);

			auto it = upper_edge.begin();
			int x = 0;
			int y_offset = 0; // If we overlap with the sprite(s) ahead we have to correct the y coordinate

			{ // We want to find the edge with the smallest y coordinate that fits
				int x2 = 0;
				auto it2 = it;
				do {
					// Could it fit in the first place?
					if (x2 + sprite_entry->sprite.w <= width) {
						// Calculate y offset if we collide with the segments up front
						auto it3 = it2;
						int x3 = it3->w;
						int y_offset2 = 0;

						while (++it3 != upper_edge.end() && x3 < sprite_entry->sprite.w) {
							y_offset2 = std::fmaxl(y_offset2, it3->y - it2->y);
							x3 += it3->w;
						}

						// Check if the result is placed lower, update variables
						if (it2->y + y_offset2 < it->y + y_offset) {
							it = it2;
							x = x2;
							y_offset = y_offset2;
						}
					} else break; // Since they are ordered the next one wont fit either

					x2 += it2->w;
				} while (++it2 != upper_edge.end());

				// We found a line to place the texture on
				it->y += y_offset; // Update the y offset of the line

				height = std::fmaxl(it->y + sprite_entry->sprite.h, height);
				if (height > glvars.max_tex_size) { // Can't fit the sprite, try a smaller one
					height = glvars.max_tex_size;
					
					entry++;
					continue;
				}

				sprite_entry->x = x;
				sprite_entry->y = it->y;
			}

			{ // Create a new Scanline for the sprite
				Scanline line { it->y + sprite_entry->sprite.h, sprite_entry->sprite.w };

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
			}

			stitched_textures.push_back(sprite_entry);
			sprites.erase(entry++); // Remove from the queue

			/*cout << "Scanline size: " << upper_edge.size() << endl;
			for (Scanline& sl : upper_edge) {
			cout << "w: " << sl.w << " y:" << sl.y << endl;
			}
			cout << endl;*/
		}

#if FORCE_POT
		height = next_power_of_two(height);
#else
		// Squeeze tight!
		for (auto line = upper_edge.rbegin(); line != upper_edge.rend(); line++) {
			if (line->y != 0) break;
			else width -= line->w;
		}
#endif

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

		for(auto entry : stitched_textures) {
			int w, h;
			GLuint texture = create_texture(entry->path, (Uint*)&w, (Uint*)&h);
			draw_gl_texture(texture, entry->x, entry->y + h, w, -h);
			Sprite& sprite = entry->sprite;

			// Calculate UVs
			sprite.vmin = entry->x / (float)width;
			sprite.umin = entry->y / (float)height;
			sprite.vmax = (entry->x + w) / (float)width;
			sprite.umax = (entry->y + h) / (float)height;

			sprite.gl_texture = gl_texture;

			/*cout << "Sprite: " << entry->path << endl;
			cout << "width: " << w << " height: " << h << endl;
			cout << "x: " << entry->x << " y: " << entry->y << endl;
			cout << "vmin " << sprite.vmin << endl;
			cout << "umin " << sprite.umin << endl;
			cout << "vmax " << sprite.vmax << endl;
			cout << "umax " << sprite.umax << endl;*/
			entry++;

			glDeleteTextures(1, &texture);
		}

		gl_textures.push_back(gl_texture);

#if WRITE_ATLAS_TO_FILE
		{
			// Write to output
			Uint8* pixel_buffer = new Uint8[width * height * 4];

			glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glReadBuffer(GL_COLOR_ATTACHMENT0);
			glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixel_buffer);

			SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormatFrom(pixel_buffer, width, height, 8, width * 4, SDL_PIXELFORMAT_RGBA32);
			string tmpfile = std::tmpnam(nullptr);
			SDL_SaveBMP(surface, (WRITE_ATLAS_OUTPUT_FILE + tmpfile + ".bmp").c_str());

			SDL_FreeSurface(surface);
			delete[] pixel_buffer;
		}
#endif

		bind_framebuffer(curr_fbo);
		glDeleteFramebuffers(1, &fbo); // Throw away framebuffer

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}

	void TextureAtlas::load() {

		// We have to rebuild the cache
		// First we have to sort the sprites by their surface area

		list<const SpriteEntry*> sprite_queue;
		for (auto& entry : sprites) sprite_queue.push_back(&entry);
			
		sprite_queue.sort([](const SpriteEntry* a, const SpriteEntry* b) {
			return a->sprite.w * a->sprite.h > b->sprite.w * b->sprite.h;
		});
		
		while (!sprite_queue.empty()) {
			create_bucket(sprite_queue);
		}
	}

	void TextureAtlas::destroy() {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
		glDeleteTextures(gl_textures.size(), gl_textures.data());
	}
}