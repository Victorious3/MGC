#pragma once
// Precompiled header, put all your
// heavy library dependencies here

#include <string>
#include <iostream>

// Libraries
extern "C" {
#include <SDL.h>
#define GLEW_STATIC
#include <GL/glew.h>
}

#include <lua.hpp>
#include <sol.hpp>

using namespace std;

typedef unsigned int Uint;

namespace constants {
	const string APP_NAME = "Magical Girl City";
	const string APP_VERSION = "0.0.0.0";

	const Uint SCR_WIDTH = 640;
	const Uint SCR_HEIGHT = 360;

	const Uint FRAMERATE = 30;
}
