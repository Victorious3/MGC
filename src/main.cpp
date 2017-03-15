#include <stdafx.h>

#include "mgc.h"
#include "font.h"
#include "ini.h"
#include "log.h"

using namespace mgc;

extern "C"
int main(int argc, char* argv[])
{
	try {
		init();
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

	destroy();

	return 0;
}