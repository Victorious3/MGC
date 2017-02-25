#include "stddef.h"

#include "mgc.h"

namespace mgc {

	SDL_Window* window = nullptr;
	SDL_GLContext context = nullptr;

	Mouse mouse;
	Keyboard key;

	bool running = true;
	bool fullscreen = false;

	void run() {
		while (running) {
			// Handle sdl events
			sdl_event();
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
		/*if (!SDL_SetWindowFullscreen(window, fullscreen ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP)) {
			fullscreen = !fullscreen;
		} else {
			SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Failed to change display mode");
		}*/
	}

	void render() {
		static Uint8 xOff;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0, 0, 0, 0);
		glColor4ub(255, 0, 0, 0);
		glBegin(GL_QUADS);
			glVertex2i(0, 0);
			glVertex2i(xOff, 0);
			glVertex2i(xOff, constants::SCR_HEIGHT);
			glVertex2i(0, constants::SCR_HEIGHT);
		glEnd();
		SDL_GL_SwapWindow(window);
		xOff++;
	}

	static void setup_projection() {
		glShadeModel(GL_FLAT);
		glClearColor(0, 0, 0, 0);
		glViewport(0, 0, constants::SCR_WIDTH, constants::SCR_HEIGHT);
		gluOrtho2D(0, constants::SCR_WIDTH, 0, constants::SCR_HEIGHT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		GLenum error = glGetError();
		if (error) {
			SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "GL error when setting up projection: %s", glewGetErrorString(error));
		}
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

		//SDL_ShowCursor(false);
		setup_projection();
	}

	void destroy_sdl() {
		if (window) SDL_DestroyWindow(window);
		if (context) SDL_GL_DeleteContext(context);
		SDL_Quit();
	}

	void init_lua() {

	}
	
	void destroy_lua() {

	}
}
