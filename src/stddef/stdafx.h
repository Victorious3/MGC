#pragma once

#pragma warning(push, 0) // Ignore warnings for external libraries
// TODO Should add something similar for other compilers

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#ifndef WIN32
#define WIN32
#endif
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
#include <SDL_image.h>
#define GLEW_STATIC
#include <GL/glew.h>
}

#include <lua.hpp>
#include <sol.hpp>

#pragma warning(pop)

using std::string;
using namespace std::literals::string_literals;

using std::vector;
using std::list;
using std::function;
using std::map;

using std::exception;

using std::cout;
using std::cerr;
using std::cin;
using std::endl;

typedef unsigned int Uint;
typedef int64_t int64;
typedef int32_t int32;
typedef int16_t int16;
typedef int8_t int8;

#ifdef NDEBUG
#define RUNTIME_ERROR($str) std::runtime_error($str)
#else
#define RUNTIME_ERROR($str) \
	std::runtime_error(string(__FILE__) + "(" + std::to_string(__LINE__) + ") " + ($str))
#endif

namespace constants {
	const string APP_NAME = "Magical Girl City";
	const string APP_VERSION = "0.0.0.0";

	const Uint SCR_WIDTH = 640;
	const Uint SCR_HEIGHT = 360;

	const Uint FRAMERATE = 60;
	const Uint TICKRATE = 30;
}

static inline string string_trim(const string& input) {
	if (input.length() == 0) return string(input);
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
// Convert a wide Unicode string to a UTF8 string
string utf8_encode(const std::wstring& wstr);

// Convert a UTF8 string to a wide Unicode String
std::wstring utf8_decode(const string& str);
#endif
