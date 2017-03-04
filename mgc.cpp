#include "stddef.h"

#include "mgc.h"
#include "font.h"

#include "keyboard.h"
#include "ini.h"

namespace mgc {

	SDL_Window* window = nullptr;
	SDL_GLContext context = nullptr;
	Canvas canvas;

	Mouse mouse;
	Graphics graphics;
	Timing timing;
	ini::IniFile ini("mgc.ini");
	Keyboard keyboard(ini);

	bool running = true;
	bool fullscreen = false;

	float scale = 1;
	Uint xoff = 0;
	Uint yoff = 0;

	Uint screen_fbo = 0;
	Uint screen_texture = 0;

	const Keyboard::Input_Action& key_fullscreen = keyboard.get_action(Keyboard::ACTIONS::TOGGLE_FULLSCREEN);

	void run() {
		while (running) {
			while (SDL_TICKS_PASSED(SDL_GetTicks(), timing.tick_last + timing.tick_delay_ms)) {
				update();
				timing.tick_last += timing.tick_delay_ms;
			}

			render();
		}
	}

	void sdl_event() {
		SDL_Event event;

		mouse.clicked = mouse.moved = false;
		mouse.wheel = mouse.xrel = mouse.yrel = 0;

		while (SDL_PollEvent(&event)) {

			bool mdown = mouse.down;

			switch (event.type) {
			case SDL_QUIT:
				running = false;
				break;
			case SDL_MOUSEMOTION:
				mouse.xrel = event.motion.xrel;
				mouse.yrel = event.motion.yrel;
				mouse.moved = true;
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
				{
				keyboard.process_sdl_event(event);
				/*
				key.state = event.key.state;
				key.cur = event.key.keysym.sym;
				key.down = key.state && !kstate;

				switch (event.key.keysym.sym) {
				case SDLK_UP: key.key_up = key.typed; break;
				case SDLK_DOWN: key.key_down = key.typed; break;
				case SDLK_LEFT: key.key_left = key.typed; break;
				case SDLK_RIGHT: key.key_right = key.typed; break;*/
				}
				break;
			case SDL_TEXTINPUT:
				/*if (key.typed) {
					key.text += event.text.text; // Handle multiple keys being pressed at once for text input
				} else {
					key.text = event.text.text;
				}
				key.typed = true;*/
				break;
			}
		}

		// Update game here

		if (key_fullscreen.pressed()) {
			toggle_fullscreen();
		}

		//if (key.typed)
		//	cout << "Keystroke: " << key.cur << ", " << key.text << endl;
		if (mouse.clicked)
			cout << "Mouse clicked: " << mouse.x << ", " << mouse.y << endl;
		if (mouse.wheel)
			cout << "Mouse wheel moved: " << mouse.wheel << endl;
		//if (mouse.moved)
		//	cout << "Mouse moved: " << mouse.x << ", " << mouse.y << endl;
	}

	static void update_resolution() {
		int w, h;
		SDL_GetWindowSize(window, &w, &h);
		scale = std::fminf((float)w / (float)constants::SCR_WIDTH, (float)h / (float)constants::SCR_HEIGHT);
	}
	
	void toggle_fullscreen() {
		if (!SDL_SetWindowFullscreen(window, fullscreen ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP)) {
			fullscreen = !fullscreen;
			update_resolution();
		} else {
			SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Failed to change display mode");
		}
	}

	void update() {
		// Handle sdl events
		keyboard.update();
		sdl_event();
	}

	void render() {
		graphics.frame_counter++;

		// Update the mouse position
		SDL_GetMouseState(&mouse.x, &mouse.y);
		mouse.x = (int)(std::floor(mouse.x / scale));
		mouse.y = (int)(std::floor(mouse.y / scale));

		if (SDL_GetTicks() - graphics.last_count_taken > 1000) {
			graphics.last_count_taken = SDL_GetTicks();
			graphics.framerate_actual = (graphics.framerate_actual + graphics.frame_counter) / 2.0f;
			graphics.frame_counter = 0;
		}
		Uint32 startTime = SDL_GetTicks();

		// Bind framebuffer for rendering

		glBindFramebuffer(GL_FRAMEBUFFER, screen_fbo);

		glViewport(0, 0, constants::SCR_WIDTH, constants::SCR_HEIGHT);

		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0, 0, 0, 0);
		glColor3ub(255, 255, 255);

		static Texture test("Resources/sprites/test.png");
		test.draw(canvas, 10, 10);

		glTranslatef((GLfloat)mouse.x, (GLfloat)mouse.y, 0);

		glBegin(GL_QUADS);
		{
			glVertex2i(1, -1);
			glVertex2i(4, -1);
			glVertex2i(4, 1);
			glVertex2i(1, 1);

			glVertex2i(-4, -1);
			glVertex2i(-1, -1);
			glVertex2i(-1, 1);
			glVertex2i(-4, 1);

			glVertex2i(-1, 1);
			glVertex2i(-1, 4);
			glVertex2i(1, 4);
			glVertex2i(1, 1);

			glVertex2i(-1, -4);
			glVertex2i(-1, -1);
			glVertex2i(1, -1);
			glVertex2i(1, -4);
		}
		glEnd();
		glTranslatef((GLfloat)-mouse.x, (GLfloat)-mouse.y, 0);

		glBegin(GL_LINES);
		{
			glVertex2i(0, 0);
			glVertex2i(mouse.x, mouse.y);
		}
		glEnd();

		string fps_string = "FPS: "s + std::to_string(graphics.framerate_actual);

		static Font montserrat("Resources/fonts/Montserrat");
		montserrat.draw_string(canvas, fps_string, 0, 0);

		// Draw to screen
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		int w, h;
		SDL_GetWindowSize(window, &w, &h);
		glViewport(0, 0, w, h);

		glColor4ub(255, 255, 255, 255);
		glEnable(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, screen_texture);
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0, 1);
			glVertex2i(0, 0);
			glTexCoord2f(1, 1);
			glVertex2i(constants::SCR_WIDTH, 0);
			glTexCoord2f(1, 0);
			glVertex2i(constants::SCR_WIDTH, constants::SCR_HEIGHT);
			glTexCoord2f(0, 0);
			glVertex2i(0, constants::SCR_HEIGHT);
		}
		glEnd();

		glDisable(GL_TEXTURE_2D);

		SDL_GL_SwapWindow(window);

		if (int i = glGetError()) {
			cout << "GL ERROR " << i << ": " << glewGetErrorString(i) << endl;
		}

		/*if (SDL_GetTicks() - startTime < graphics.frame_delay_ms) {
			SDL_Delay(graphics.frame_delay_ms - (SDL_GetTicks() - startTime));
		}*/
	}

	static void setup_gl() {
		// Initializing glew
		glewInit();
		// Setup framebuffer for scaling
		glGenFramebuffers(1, &screen_fbo);
		glGenTextures(1, &screen_texture);

		glBindTexture(GL_TEXTURE_2D, screen_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, constants::SCR_WIDTH, constants::SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		
		glBindFramebuffer(GL_FRAMEBUFFER, screen_fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screen_texture, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Framebuffer creation faled");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glShadeModel(GL_FLAT);
		glClearColor(0, 0, 0, 0);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glViewport(0, 0, constants::SCR_WIDTH, constants::SCR_HEIGHT);
		gluOrtho2D(0, constants::SCR_WIDTH, constants::SCR_HEIGHT, 0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glDisable(GL_CULL_FACE);

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

	void setup_image() {
		int imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags)) {
			throw runtime_error("Image_Init Error: could not initialize SDL_IMAGE. Error: "s + IMG_GetError() + "\n");
		}
	}

	void setup_graphics() {
		graphics.framerate_actual = constants::FRAMERATE;
		graphics.frame_delay_ms = (Uint32)(1000.0f / constants::FRAMERATE);
	}

	void setup_timing() {
		timing.tick_delay_ms = (1000 / constants::TICKRATE);
		timing.tick_counter = 0;
		timing.tick_last = SDL_GetTicks();
	}

	void setup_input() {
		//keyboard.read_config(ini);
		//key_fullscreen = keyboard.get_action(mgc::Keyboard::ACTIONS::TOGGLE_FULLSCREEN);
	}

	void init_sdl() {
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS)) {
			throw runtime_error("SDL_Init Error: "s + SDL_GetError());
		}
		SDL_Log("SDL initialized successfully");

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

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
		setup_gl();

		setup_ttf();
		setup_image();
		setup_graphics();
		setup_timing();

		setup_input();
	}

	void destroy_graphics() {
		texture_manager.destroy_all();

		IMG_Quit();
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
