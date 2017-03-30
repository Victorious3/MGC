#include <stdafx.h>

#include "mgc.h"
#include "log.h"

#include "TextureAtlas.h"

using namespace mgc;

extern "C"
int main(int argc, char* argv[])
{
	try {
		init();

		render::TextureAtlas test_atlas("Resources/sprites/testatlas.bmp");
		test_atlas.add_sprite("Resources/sprites/test.png");
		test_atlas.add_sprite("Resources/sprites/tsprite1.png");
		test_atlas.add_sprite("Resources/sprites/tsprite2.png");
		test_atlas.add_sprite("Resources/sprites/tsprite3.png");
		test_atlas.add_sprite("Resources/sprites/tsprite4.png");
		test_atlas.add_sprite("Resources/sprites/tsprite5.png");
		test_atlas.add_sprite("Resources/sprites/tsprite6.png");
		test_atlas.load();

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