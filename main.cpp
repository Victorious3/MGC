#include "stddef.h"

#include "mgc.h"

int main(int argc, char* argv[])
{
	try {
		try {
			mgc::init_sdl();
			mgc::init_lua();
		}
		catch (exception& e) {
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, e.what());
			goto finalize;
		}

		mgc::run();
	}
	catch (exception& e) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "An exception occured %s", e.what());
	}
	catch (...) {
		// Unexpected error
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "An unexpected error occured");
	}

finalize:
	SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Shutting down!");
	
	mgc::destroy_sdl();
	mgc::destroy_lua();

	return 0;
}