#include <stdafx.h>

#include "mgc.h"
#include "log.h"
#include "font.h"
#include "keyboard.h"
#include "ini.h"
#include "Locale.h"
#include "ui/UI.h"
#include "ui/Image.h"

namespace mgc {

	SDL_Window* window = nullptr;
	SDL_GLContext context = nullptr;

	Mouse mouse;
	Graphics graphics;
	Timing timing;
	ini::IniFile ini("mgc.ini");
	Keyboard keyboard;
	Locale loc("en-us");

	bool running = true;
	bool fullscreen = false;

	float scale = 1;
	Uint xoff = 0;
	Uint yoff = 0;

	Uint screen_fbo = 0;
	Uint screen_texture = 0;

	static const Keyboard::InputAction& key_fullscreen = keyboard.get_action("toggle_fullscreen", SDL_SCANCODE_F11);

	static void sdl_event() {
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

		if (key_fullscreen.fired) {
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
			log::error << "Failed to change display mode" << endl;
		}
	}

	void update() {
		// Handle sdl events
		keyboard.update();
		sdl_event();
		UI::update();
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
		montserrat.draw_string(fps_string, 0, 0);
		montserrat.draw_string( 
			"I'm just your average school slut, don't mind me.\n" 
			"Just pretend I had some special narration and\n"
			"dialog for you.\n\n"
			"So they let me come up with some shitty lorem\n"
			"ipsum text. As if they couldn't do that themselves.\n" 
			"Lazy fucks.\n"
			"Can all go to hell if it was after me. Skanks.\n"
			"(Don't let Vic write a dialog)", 10, 50);

		montserrat.draw_string(loc.get_string("test_string"), 0, 30);

		UI::render();

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

	void run() {
		while (running) {
			while (SDL_TICKS_PASSED(SDL_GetTicks(), timing.tick_last + timing.tick_delay_ms)) {
				update();
				timing.tick_last += timing.tick_delay_ms;
			}

			render();
		}
	}

	// Initialization code

	static void init_gl() {
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
			log::error << "Framebuffer creation faled" << endl;
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
			log::error << "GL error when setting up projection: " << glewGetErrorString(error);
		}
	}

	static void init_image() {
		int imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags)) {
			throw RUNTIME_ERROR("Image_Init Error: could not initialize SDL_IMAGE. Error: "s + IMG_GetError() + "\n");
		}
	}

	static void init_graphics() {
		graphics.framerate_actual = constants::FRAMERATE;
		graphics.frame_delay_ms = (Uint32)(1000.0f / constants::FRAMERATE);
	}

	static void init_timing() {
		timing.tick_delay_ms = (1000 / constants::TICKRATE);
		timing.tick_counter = 0;
		timing.tick_last = SDL_GetTicks();
	}

	static void init_input() {
		keyboard.read_config(ini, "key bindings");
	}

	static void init_sdl() {
		static const string category_names[]{
			"Application",
			"Error",
			"Assert",
			"System",
			"Audio",
			"Video",
			"Render",
			"Input",
			"Test"
		};

		static const string priority_names[]{
			"???",
			"VERBOSE",
			"DEBUG",
			"INFO",
			"WARN",
			"ERROR",
			"CRITICAL"
		};

		SDL_LogSetOutputFunction([](void* userdata, int category, SDL_LogPriority priority, const char* message) {
			string category_name = category < 0 || category >= std::extent<decltype(category_names)>::value ? "???" : category_names[category];
			string priority_name = priority < 0 || priority >= std::extent<decltype(priority_names)>::value ? "CRITICAL" : priority_names[priority];

			if (priority >= SDL_LOG_PRIORITY_ERROR)
				cerr << priority_name << " [" << category_name << "]: " << message << endl;
			else
				cout << priority_name << " [" << category_name << "]: " << message << endl;

		}, nullptr);

		SDL_LogSetAllPriority(static_cast<SDL_LogPriority>(0)); // I have to cast here to get all messages


		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS)) {
			throw RUNTIME_ERROR("SDL_Init Error: "s + SDL_GetError());
		}

		window = SDL_CreateWindow(
			constants::APP_NAME.c_str(), // title
			SDL_WINDOWPOS_CENTERED,	// xpos
			SDL_WINDOWPOS_CENTERED, // ypos
			constants::SCR_WIDTH,	// width
			constants::SCR_HEIGHT,	// height
			SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL // flags, we force OpenGL
		);

		if (!window) {
			throw RUNTIME_ERROR("SDL_CreateWindow Error: "s + SDL_GetError());
		}

		context = SDL_GL_CreateContext(window);
		if (!context) {
			throw RUNTIME_ERROR("SDL_GL_CreateContext Error: "s + SDL_GetError());
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		SDL_GL_SetSwapInterval(1);

		SDL_ShowCursor(false);

		log::info << "SDL initialized successfully" << endl;
	}

	void init_ui() {
		UI::push_element(new UI::Image(450, 10, "Resources/sprites/test.png", render::texture_manager));
	}

	// Finalization code

	static void destroy_graphics() {
		render::texture_manager.destroy_all();

		IMG_Quit();
	}

	static void destroy_sdl() {
		if (window) SDL_DestroyWindow(window);
		if (context) SDL_GL_DeleteContext(context);
		SDL_Quit();
	}

	void init() {
		init_sdl();
		init_gl();
		init_image();
		init_graphics();
		init_timing();
		init_input();
		init_ui();
	}

	void destroy() {
		destroy_graphics();
		destroy_sdl();
	}
}
