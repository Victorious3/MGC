#include <stdafx.h>

#include "mgc.h"
#include "log.h"
#include "keyboard.h"
#include "Locale.h"

#include "ini/ini.h"
#include "render/font.h"
#include "render/shader.h"
#include "render/VertexBuffer.h"

#include "ui/UI.h"
#include "ui/Image.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

namespace mgc {

	Window window;
	Mouse mouse;
	Graphics graphics;
	Timing timing;
	Keyboard keyboard;

	ini::IniFile main_config("mgc.ini");
	Locale locale("en-us");

	bool running = true;

	static const Keyboard::InputAction& key_fullscreen      = keyboard.get_action("toggle_fullscreen", SDL_SCANCODE_F11);
	static const Keyboard::InputAction& key_reload_textures = keyboard.get_action("reload_textures", SDL_SCANCODE_F1);
	static const Keyboard::InputAction& key_reload_shaders  = keyboard.get_action("reload_shaders", SDL_SCANCODE_F2);


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

#ifndef NDEBUG
		// Debug keys
		if (key_reload_textures.fired) {
			render::texture_manager.reload();
		}
		if (key_reload_shaders.fired) {
			
		}
#endif

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
		SDL_GetWindowSize(window.sdl_window, &w, &h);
		window.scale = std::fminf((float)w / (float)constants::SCR_WIDTH, (float)h / (float)constants::SCR_HEIGHT);
	}
	
	void toggle_fullscreen() {
		if (!SDL_SetWindowFullscreen(window.sdl_window, window.fullscreen ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP)) {
			window.fullscreen = !window.fullscreen;
			update_resolution();
		} else {
			log::error << "Failed to change display mode" << endl;
		}
	}

	void update() {
		timing.ticks_delta_ms += timing.time_delta;
	
		{ // Update stuff with delta time
			keyboard.update(timing.time_delta);
			sdl_event();
			UI::update(timing.time_delta);
		}

		{ // Update tick intervals
			while (timing.ticks_delta_ms >= timing.tick_delay_ms) {
				timing.ticks++;
				timing.ticks_delta_ms -= timing.tick_delay_ms;

				tick();
			}
		}
	}

	void tick() {

	}

	void render() {
		glm::mat4 projection = glm::ortho(0.0F, (float) constants::SCR_WIDTH, (float) constants::SCR_HEIGHT, 0.0F);
		render::core_shader();
		glUniformMatrix4fv(render::core_shader.projection, 1, false, glm::value_ptr(projection));
		glUniformMatrix4fv(render::core_shader.projection, 1, false, glm::value_ptr(projection));

		graphics.render_delta_ms += timing.time_delta;
		Uint64 time_now = time_millis();

		if (time_now - graphics.last_count_taken > 1000) {
			graphics.last_count_taken = time_now;
			graphics.framerate_actual = (graphics.framerate_actual + graphics.frame_counter) / 2.0f;
			graphics.frame_counter = 0;
		}

		// Only render when we need to
		if (graphics.render_delta_ms < graphics.frame_delay_ms)
			return;

		graphics.frame_counter++;

		// Update the mouse position
		SDL_GetMouseState(&mouse.x, &mouse.y);
		mouse.x = (int)(std::floor(mouse.x / window.scale));
		mouse.y = (int)(std::floor(mouse.y / window.scale));

		
		Uint32 startTime = SDL_GetTicks();

		// Bind framebuffer for rendering
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindFramebuffer(GL_FRAMEBUFFER, window.screen_fbo);
		glViewport(0, 0, constants::SCR_WIDTH, constants::SCR_HEIGHT);

		glClear(GL_COLOR_BUFFER_BIT);

		/*
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

		montserrat.draw_string(locale.get_string("test_string"), 0, 30);

		UI::render();
		*/

		// "Mouse cursor"

		render::VertexBuffer vb;

		/*vb.quad(mouse.x - 5, mouse.y - 1, 4, 3);
		vb.quad(mouse.x + 1, mouse.y - 1, 4, 3);
		vb.quad(mouse.x - 1, mouse.y - 5, 3, 4);
		vb.quad(mouse.x - 1, mouse.y + 1, 3, 4);
		vb.draw_imm();*/

		// Draw to screen
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		int w, h;
		SDL_GetWindowSize(window.sdl_window, &w, &h);
		glViewport(0, 0, w, h);

		vb.set_texture(window.screen_texture);
		//vb.sprite(0, 0, render::Sprite { constants::SCR_WIDTH, constants::SCR_HEIGHT });
		//vb.quad(0, 0, 20, 40);
		glm::vec4 v1 = glm::vec4 { glm::ivec2 { 0, constants::SCR_HEIGHT }, 0, 1 };
		glm::vec4 v2 = glm::vec4 { glm::ivec2 { 0, 0 }, 0, 1 };
		glm::vec4 v3 = glm::vec4 { glm::ivec2 { constants::SCR_WIDTH / 2, constants::SCR_HEIGHT / 2 }, 0, 1 };

		vb.vertex(v1.x, v1.y);
		vb.vertex(v2.x, v2.y);
		vb.vertex(v3.x, v3.y);
		vb.draw_imm();

		cout << glm::to_string(projection) << endl;
		//cout << glm::to_string(projection * glm::vec4(constants::SCR_WIDTH / 2, constants::SCR_HEIGHT / 2, 0, 1)) << endl;

		SDL_GL_SwapWindow(window.sdl_window);

		if (GLenum err = glGetError()) {
			log::error << "GL ERROR " << err << ": " << gluErrorString(err) << endl;
		}

		/*if (SDL_GetTicks() - startTime < graphics.frame_delay_ms) {
			SDL_Delay(graphics.frame_delay_ms - (SDL_GetTicks() - startTime));
		}*/
	}

	void run() {
		while (running) {
			Uint64 time_now = time_millis();
			timing.time_delta = time_now - timing.time_last;

			update();
			render();

			timing.time_last = time_now;
		}
	}

	// Initialization code

	static void init_gl() {
		// Initializing glew
		if (GLenum error = glewInit()) {
			throw RUNTIME_ERROR("Error while setting up GLEW: "s + reinterpret_cast<const char*>(glewGetErrorString(error)));
		}
		render::init_glvars();

		// Setup framebuffer for scaling
		window.screen_texture = render::allocate_texture(constants::SCR_WIDTH, constants::SCR_HEIGHT);
		window.screen_fbo = render::create_framebuffer(window.screen_texture);

		glClearColor(0, 0, 0, 0);
		glViewport(0, 0, constants::SCR_WIDTH, constants::SCR_HEIGHT);

		glDisable(GL_CULL_FACE);
		/*glFrontFace(GL_CW); // Clockwise sounds natural
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);*/

		render::try_throw_gl_error("GL error when setting up projection");

		render::load_shaders();
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
		timing.ticks = 0;
		timing.time_last = time_millis();
		timing.ticks_delta = 0;
		timing.time_delta = 0;
		timing.ticks_delta_ms = 0;
	}

	static void init_input() {
		keyboard.read_config(main_config, "key bindings");
	}

	static void init_sdl() {
		{ // SDL log
			static const string category_names[] {
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

			static const string priority_names[] {
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
		}

		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS)) {
			throw RUNTIME_ERROR("SDL_Init Error: "s + SDL_GetError());
		}

		window.sdl_window = SDL_CreateWindow(
			constants::APP_NAME.c_str(), // title
			SDL_WINDOWPOS_CENTERED,	// xpos
			SDL_WINDOWPOS_CENTERED, // ypos
			constants::SCR_WIDTH,	// width
			constants::SCR_HEIGHT,	// height
			SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL // flags, we force OpenGL
		);

		if (!window.sdl_window) {
			throw RUNTIME_ERROR("SDL_CreateWindow Error: "s + SDL_GetError());
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		SDL_GL_SetSwapInterval(1);

		window.sdl_context = SDL_GL_CreateContext(window.sdl_window);
		if (!window.sdl_context) {
			throw RUNTIME_ERROR("SDL_GL_CreateContext Error: "s + SDL_GetError());
		}

		SDL_ShowCursor(false);

		log::info << "SDL initialized successfully" << endl;
	}

	void init_ui() {
		//UI::push_element(new UI::Image(450, 10, "Resources/sprites/test.png", render::texture_manager));
	}

	static void init_lua() {
		
	}

	// Finalization code

	static void destroy_graphics() {
		render::texture_manager.destroy_all();
		render::destroy_shaders();

		IMG_Quit();
	}

	static void destroy_sdl() {
		if (window.sdl_window) SDL_DestroyWindow(window.sdl_window);
		if (window.sdl_context) SDL_GL_DeleteContext(window.sdl_context);
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
		init_lua();
	}

	void destroy() {
		destroy_graphics();
		destroy_sdl();
	}
}
