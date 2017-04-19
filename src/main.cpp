#include <stdafx.h>

#include "mgc.h"
#include "log.h"

#include "render/TextureAtlas.h"

using namespace mgc;

extern "C"
int main(int argc, char* argv[])
{
	try {
		std::set_terminate([] {
			cerr << "Fatal error, unexpected termination!" << endl;
			std::abort();
		});

		init();
		run();
	}
	catch (exception& e) {
		log::error << "An exception occured: " << e.what() << endl;
#ifdef WIN32
		MessageBox(nullptr, utf8_decode(string(e.what()) + "\n\nPlease report to https://github.com/Victorious3/MGC").c_str(), L"Exception", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
#else
		cin.ignore();
#endif
	}
	catch (...) {
		// Unexpected error
		log::error << "An unexpected error occured!" << endl;
#ifdef WIN32
		MessageBox(nullptr, L"Unexpected Error!\n\nPlease report to https://github.com/Victorious3/MGC", L"Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
#else
		cin.ignore();
#endif
	}

	log::critical << "Shutting down!" << endl;

	destroy();

	return 0;
}