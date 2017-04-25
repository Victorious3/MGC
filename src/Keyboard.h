#pragma once
#include <stdafx.h>

#include "ini/ini.h"

namespace mgc {

	class Keyboard final
	{
	public:
		class InputAction {
		public:
			bool active; // True if the action is currently active (key pressed, etc)
			bool fired; // True if the action changed state to active (key typed, etc)
		private:
			friend Keyboard;
			InputAction(SDL_Scancode scancode) : scancode(scancode) {}
			SDL_Scancode scancode; // We only support single keys for now, interface is open for multiple
		};

		Keyboard();
		Keyboard(const ini::IniFile& ini, const string& section_name);

		void update(Uint64 delta_ms);
		void read_config(const ini::IniFile& ini, const string& section_name);
		void process_sdl_event(SDL_Event& key_event);

		const InputAction& get_action(const string& name, SDL_Scancode default);
	private:
		map<const string, InputAction> actionmap;
	};
}