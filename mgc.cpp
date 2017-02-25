#include "stddef.h"

#include "mgc.h"

namespace mgc {

	SDL_Window* window = nullptr;
	SDL_GLContext context = nullptr;

	Mouse mouse;
	Keyboard key;
	Graphics graphics;
	Timing timing;

	bool running = true;
	bool fullscreen = false;
	float scale = 1;

	void run() {
		while (running) {
			sdl_event();

			while (SDL_TICKS_PASSED(SDL_GetTicks(), timing.tick_last + timing.tick_delay_ms)) {
				update();
				timing.tick_last += timing.tick_delay_ms;
			}

			// Handle sdl events
			render();
		}
	}

	void sdl_event() {
		SDL_Event event;

		mouse.clicked = key.typed = key.down = mouse.moved = false;
		mouse.wheel = mouse.xrel = mouse.yrel = 0;

		while (SDL_PollEvent(&event)) {

			bool mdown = mouse.down;
			bool kstate = key.state;

			switch (event.type) {
			case SDL_QUIT:
				running = false;
				break;
			case SDL_MOUSEMOTION:
				mouse.x = event.motion.x;
				mouse.y = event.motion.y;
				mouse.xrel = event.motion.xrel;
				mouse.yrel = event.motion.yrel;
				mouse.moved = mouse.xrel || mouse.yrel;
				break;
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				mouse.down = event.button.state;
				mouse.clicked = !mdown && mouse.down;
				break;
			case SDL_MOUSEWHEEL:
				mouse.wheel = event.wheel.y;
				break;
			case SDL_KEYDOWN:
			case SDL_KEYUP: 
				key.state = event.key.state;
				key.cur = event.key.keysym.sym;
				key.down = key.state && !kstate;

				switch (event.key.keysym.sym) {
				case SDLK_UP: key.key_up = key.typed; break;
				case SDLK_DOWN: key.key_down = key.typed; break;
				case SDLK_LEFT: key.key_left = key.typed; break;
				case SDLK_RIGHT: key.key_right = key.typed; break;
				}
				break;
			case SDL_TEXTINPUT:
				if (key.typed) {
					key.text += event.text.text; // Handle multiple keys being pressed at once for text input
				} else {
					key.text = event.text.text;
				}
				key.typed = true;
				break;
			}
		}

		// Update game here

		if (key.down) {
			if (key.cur == SDLK_F11) {
				toggle_fullscreen();
			}
		}

		if (key.typed)
			cout << "Keystroke: " << key.cur << ", " << key.text << endl;
		if (mouse.clicked)
			cout << "Mouse clicked: " << mouse.x << ", " << mouse.y << endl;
		if (mouse.wheel)
			cout << "Mouse wheel moved: " << mouse.wheel << endl;
		if (mouse.moved)
			cout << "Mouse moved: " << mouse.x << ", " << mouse.y << endl;
	}

	static void update_resolution() {

	}
	
	void toggle_fullscreen() {
		if (!SDL_SetWindowFullscreen(window, fullscreen ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP)) {
			fullscreen = !fullscreen;
		} else {
			SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Failed to change display mode");
		}
	}

	void update() {

	}

	void render() {
		graphics.frame_counter++;

		if (SDL_GetTicks() - graphics.last_count_taken > 1000) {
			graphics.last_count_taken = SDL_GetTicks();
			graphics.framerate_actual = (graphics.framerate_actual + graphics.frame_counter) / 2.0f;
			graphics.frame_counter = 0;
		}

		Uint32 startTime = SDL_GetTicks();

		static Uint8 xOff;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0, 0, 0, 0);
		glColor4ub(255, 0, 0, 0);
		glTranslatef(mouse.x - 5, ((GLfloat)constants::SCR_HEIGHT) - mouse.y - 5, 0);
		glBegin(GL_QUADS);
			glVertex2i(0, 0);
			glVertex2i(10, 0);
			glVertex2i(10, 10);
			glVertex2i(0, 10);
		glEnd();
		glTranslatef(-mouse.x + 5, -((GLfloat)constants::SCR_HEIGHT) + mouse.y + 5, 0);
		
		SDL_Color color = { 255, 255, 255 };
		SDL_Surface *textSurface = TTF_RenderUTF8_Blended(graphics.font_debug, ("FPS: "s + std::to_string(graphics.framerate_actual)).c_str(), color);

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
				glVertex2i(0, constants::SCR_HEIGHT - textSurface->h);
				glTexCoord2f(0, 0);
				glVertex2i(0, constants::SCR_HEIGHT);
				glTexCoord2f(1, 0);
				glVertex2i(0 + textSurface->w, constants::SCR_HEIGHT);
				glTexCoord2f(1, 1);
				glVertex2i(0 + textSurface->w, constants::SCR_HEIGHT - textSurface->h);
			}
			glEnd();

			glDisable(GL_BLEND);

			glDisable(GL_TEXTURE_2D);
			

			glDeleteTextures(1, &texture);
			SDL_FreeSurface(textSurface);
		}

		SDL_GL_SwapWindow(window);
		

		if (SDL_GetTicks() - startTime < graphics.frame_delay_ms) {
			SDL_Delay(graphics.frame_delay_ms - (SDL_GetTicks() - startTime));
		}
	}

	static void setup_projection() {
		glShadeModel(GL_FLAT);
		glClearColor(0, 0, 0, 0);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glViewport(0, 0, constants::SCR_WIDTH, constants::SCR_HEIGHT);
		gluOrtho2D(0, constants::SCR_WIDTH, 0, constants::SCR_HEIGHT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		GLenum error = glGetError();
		if (error) {
			SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "GL error when setting up projection: %s", glewGetErrorString(error));
		}
	}

	void setup_ttf() {
		if (TTF_Init() == -1) {
			throw runtime_error("TTF_Init Error: could not initialize TTF. Error: "s + TTF_GetError());
		}
	}

	void setup_graphics() {
		graphics.framerate_actual = constants::FRAMERATE;
		graphics.frame_delay_ms = (Uint32)(1000.0f / constants::FRAMERATE);

		string fontPath = "Resources/fonts/OpenSans/OpenSans-Regular.ttf";
		graphics.font_debug = TTF_OpenFont(fontPath.c_str(), 16);
		if (!graphics.font_debug) {
			throw runtime_error("Graphics_Init Error: Could not load font \"" + fontPath + "\". "s + TTF_GetError());
		}
	}

	void setup_timing() {
		timing.tick_delay_ms = (1000 / constants::TICKRATE);
		timing.tick_counter = 0;
		timing.tick_last = SDL_GetTicks();
	}

	void init_sdl() {
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS)) {
			throw runtime_error("SDL_Init Error: "s + SDL_GetError());
		}
		SDL_Log("SDL initialized successfully");

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		SDL_GL_SetSwapInterval(1);

		window = SDL_CreateWindow(
			constants::APP_NAME.c_str(), // title
			SDL_WINDOWPOS_CENTERED,	// xpos
			SDL_WINDOWPOS_CENTERED, // ypos
			constants::SCR_WIDTH,	// width
			constants::SCR_HEIGHT,	// height
			SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL // flags, we force OpenGL
		);

		if (!window) {
			throw runtime_error("SDL_CreateWindow Error: "s + SDL_GetError());
		}

		context = SDL_GL_CreateContext(window);
		if (!context) {
			throw runtime_error("SDL_GL_CreateContext Error: "s + SDL_GetError());
		}

		SDL_ShowCursor(false);
		setup_projection();

		setup_ttf();

		setup_graphics();

		setup_timing();
	}

	void destroy_graphics() {
		if (graphics.font_debug)
			TTF_CloseFont(graphics.font_debug);
		graphics.font_debug = NULL;
	}

	void destroy_sdl() {
		destroy_graphics();

		if (window) SDL_DestroyWindow(window);
		if (context) SDL_GL_DeleteContext(context);
		SDL_Quit();
	}

	void init_lua() {

	}
	
	void destroy_lua() {

	}
}
