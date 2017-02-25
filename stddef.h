#pragma once
// Precompiled header, put all your
// heavy library dependencies here

// stdlib
#include <string>
#include <vector>
#include <iostream>

// Libraries
extern "C" {
#include <SDL.h>
#include <SDL_ttf.h>
#define GLEW_STATIC
#include <GL/glew.h>
}

#include <lua.hpp>
#include <sol.hpp>

using std::string;
using namespace std::literals::string_literals;

using std::vector;

using std::exception;
using std::runtime_error;

using std::cout;
using std::cin;
using std::endl;

typedef unsigned int Uint;

namespace constants {
	const string APP_NAME = "Magical Girl City";
	const string APP_VERSION = "0.0.0.0";

	const Uint SCR_WIDTH = 640;
	const Uint SCR_HEIGHT = 360;

	const Uint FRAMERATE = 60;
	const Uint TICKRATE = 30;
}
