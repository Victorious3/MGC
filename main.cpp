#include "stddef.h"

#include "mgc.h"
#include "font.h"
#include "ini.h"

int main(int argc, char* argv[])
{
	try {
		mgc::init_sdl();
		mgc::init_lua();

		mgc::run();
		goto finalize;
	}
	catch (exception& e) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "An exception occured: %s", e.what());
		cin.ignore();
	}
	catch (...) {
		// Unexpected error
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "An unexpected error occured!");
		cin.ignore();
	}

finalize:
	SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Shutting down!");

	mgc::destroy_sdl();
	mgc::destroy_lua();

	return 0;
}