#pragma once
#include <stdafx.h>

#include "Keyboard.h"
#include "Locale.h"

#include "ini/ini.h"
#include "game/stage.h"
#include "render/render.h"
#include "render/texture.h"

namespace mgc {
	struct Window {
		// SDL window instance
		SDL_Window* sdl_window = nullptr;
		// GL context
		SDL_GLContext sdl_context = nullptr;

		float scale = 1;
		Uint xoff;
		Uint yoff;

		bool fullscreen = false;

		Uint screen_fbo = 0;
		Uint screen_texture = 0;
	};
	extern Window window;

	struct Mouse {
		int x;
		int y;
		int xrel;
		int yrel;
		int wheel;

		bool down;
		bool clicked;
		bool moved;
	};
	extern Mouse mouse;

	struct Graphics {
		float framerate_actual;
		Uint32 frame_delay_ms;
		Uint32 frame_counter;
		Uint64 last_count_taken;
		Uint64 render_delta_ms;
	};
	extern Graphics graphics;

	struct Timing {
		Uint64 time_last;
		Uint64 time_delta;
		Uint64 ticks;
		Uint64 ticks_delta;
		Uint64 ticks_delta_ms;
		Uint64 tick_delay_ms;
	};
	extern Timing timing;

	extern Keyboard keyboard;
	extern ini::IniFile main_config;
	extern Locale locale;

	extern bool running;

	// Gameplay
	extern Stage stage;
	
	// Externally visible functions

	void init();

	void toggle_fullscreen();
	void run();

	void update();
	void tick();
	void render();

	void destroy();
}