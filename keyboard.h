#pragma once

#include "stddef.h"
#include "ini.h"

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

			bool down() const {
				return _down;
			}

			bool pressed() const {
				return _pressed;
			}

			void set(bool down_state, bool pressed_state) {
				_down = down_state;
				_pressed = pressed_state;
			}
		};

		enum class ACTIONS {
			TOGGLE_FULLSCREEN
		};

		Keyboard();
		~Keyboard();

		Keyboard(const ini::IniFile& ini);

		void read_config(const ini::IniFile& ini);

		void update();

		void process_sdl_event(SDL_Event& key_event);

		const Input_Action& get_action(const ACTIONS& action) const;

	private:
		map<ACTIONS, Input_Action> actionmap;
	};
}