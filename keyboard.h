#pragma once

#include "stddef.h"

#include <map>
using std::map;

#include "ini/ini_file.h"

namespace mgc {

	class Keyboard
	{
	public:
		struct Input_Action {
		private:
			bool _down;
			bool _pressed;
		public:
			vector<Uint8> scancodes;

			bool down() {
				return _down;
			}

			bool pressed() {
				return _pressed;
			}

			void set(bool down_state, bool pressed_state) {
				_down = down_state;
				_pressed = pressed_state;
			}
		};

		enum ACTIONS {
			TOGGLE_FULLSCREEN
		};

		Keyboard();
		~Keyboard();

		Keyboard(ini::IniFile& ini);

		void read_config(ini::IniFile& ini);

		void update();

		void process_sdl_event(SDL_Event& key_event);

		Input_Action& get_action(ACTIONS action);

	private:
		map<ACTIONS, Input_Action> actionmap;
	};
}