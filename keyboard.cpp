#include "keyboard.h"

mgc::Keyboard::Keyboard() {
	// TODO used config file
	actionmap[TOGGLE_FULLSCREEN].scancodes.push_back(SDL_SCANCODE_F11);
}

mgc::Keyboard::~Keyboard() {
}

void mgc::Keyboard::process_sdl_event(SDL_Event& event) {
}

void mgc::Keyboard::update() {
	auto keyboard_state = SDL_GetKeyboardState(NULL);

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
