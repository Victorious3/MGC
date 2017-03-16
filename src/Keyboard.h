#pragma once
#include <stdafx.h>

#include "ini.h"

namespace mgc {

	class Keyboard
	{
	public:
		class InputAction {
		public:
			bool down;
			bool pressed;
		private:
			friend Keyboard;

			vector<Uint8> scancodes;
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

		const InputAction& get_action(const ACTIONS& action) const;

	private:
		map<ACTIONS, InputAction> actionmap;
	};
}