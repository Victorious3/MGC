#include "keyboard.h"

mgc::Keyboard::Keyboard() {
}

mgc::Keyboard::~Keyboard() {
}

void mgc::Keyboard::read_config(ini::Ini_File& ini) {
	actionmap[TOGGLE_FULLSCREEN].scancodes.clear();

	actionmap[TOGGLE_FULLSCREEN].scancodes.push_back((Uint8) stoi(ini.get_key_value("key bindings", "toggle_fullscreen")));
}

void mgc::Keyboard::process_sdl_event(SDL_Event& event) {
}

void mgc::Keyboard::update() {
	auto keyboard_state = SDL_GetKeyboardState(nullptr);

	for (auto& iter : actionmap) {
		for (auto& scan_iter : iter.second.scancodes) {
			if (keyboard_state[scan_iter]) {
				iter.second.set(true, !iter.second.down());
				break;
			}
			iter.second.set(false, false);
		}
	}
}

mgc::Keyboard::Input_Action& mgc::Keyboard::get_action(ACTIONS action) {
	return actionmap[action];
}
