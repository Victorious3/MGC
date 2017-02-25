#include "stddef.h"

#include "mgc.h"

int main(int argc, char* argv[])
{
	try {
		mgc::init_sdl();
		mgc::init_lua();

		mgc::run();

	} catch (exception& e) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, e.what());
	}
	
	mgc::destroy_sdl();
	mgc::destroy_lua();

	return 0;
}