#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define WIN32
#include <windows.h>
#endif

// Precompiled header, put all your
// heavy library dependencies here

// stdlib
#include <iostream>
#include <map>


// Libraries
extern "C" {
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#define GLEW_STATIC
#include <GL/glew.h>
}

#include <lua.hpp>
#include <sol.hpp>

using std::string;
using namespace std::literals::string_literals;

using std::vector;
using std::list;
using std::function;
using std::map;

using std::exception;
using std::runtime_error;

using std::cout;
using std::cerr;
using std::cin;
using std::endl;

typedef unsigned int Uint;
typedef int64_t int64;
typedef int32_t int32;
typedef int16_t int16;
typedef int8_t int8;

namespace constants {
	const string APP_NAME = "Magical Girl City";
	const string APP_VERSION = "0.0.0.0";

	const Uint SCR_WIDTH = 640;
	const Uint SCR_HEIGHT = 360;

	const Uint FRAMERATE = 60;
	const Uint TICKRATE = 30;
}

static inline string string_trim(string& input) {
	size_t first = input.find_first_not_of(' ');
	size_t last = input.find_last_not_of(' ');
	return input.substr(first, (last - first + 1));
}

static inline string string_tolower(string& input) {
	auto output = string(input);
	std::locale loc;

	for (auto& elem : output) {
		elem = std::tolower(elem, loc);
	}
	return output;
}

// Windows only

#ifdef WIN32
// Convert a wide Unicode string to an UTF8 string
string utf8_encode(const std::wstring& wstr);

// Convert an UTF8 string to a wide Unicode String
std::wstring utf8_decode(const string& str);
#endif
