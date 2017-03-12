#include "stddef.h"

#include "mgc.h"
#include "font.h"
#include "ini.h"
#include "log.h"

using namespace mgc;

int main(int argc, char* argv[])
{
	try {
		init_sdl();
		init_lua();
		run();
	}
	catch (exception& e) {
		log::error << "An exception occured: " << e.what() << endl;
		cin.ignore();
	}
	catch (...) {
		// Unexpected error
		log::error << "An unexpected error occured!" << endl;
		cin.ignore();
	}

	log::critical << "Shutting down!" << endl;

	destroy_sdl();
	destroy_lua();

	return 0;
}