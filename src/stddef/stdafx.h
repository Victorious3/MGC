#pragma once

#pragma warning(push, 0) // Ignore warnings for external libraries
// TODO Should add something similar for other compilers

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#ifndef WIN32
#define WIN32
#endif
#define NOGDI
#include <windows.h>
#endif

// Precompiled header, put all your
// heavy library dependencies here

// stdlib
#include <iostream>
#include <map>
#include <set>
#include <chrono>

// Libraries
extern "C" {
#include <SDL.h>
#include <SDL_image.h>
#define GLEW_STATIC
#include <GL/glew.h>
}

#include <glm/glm.hpp>

#include <lua.hpp>
#include <sol.hpp>

#pragma warning(pop)

using std::string;
using namespace std::literals::string_literals;

using std::vector;
using std::list;
using std::function;
using std::map;
using std::set;

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

static inline bool string_ends_with(const std::string& str, const std::string& suffix) {
	return str.size() >= suffix.size() &&
		str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
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

static inline Uint64 time_millis() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

static inline Uint next_power_of_two(Uint x) {
	x--;
	x |= x >> 1;  // handle  2 bit numbers
	x |= x >> 2;  // handle  4 bit numbers
	x |= x >> 4;  // handle  8 bit numbers
	x |= x >> 8;  // handle 16 bit numbers
	x |= x >> 16; // handle 32 bit numbers
	x++;
	return x;
}

// Windows only

#ifdef WIN32
// Convert a wide Unicode string to a UTF8 string
string utf8_encode(const std::wstring& wstr);

// Convert a UTF8 string to a wide Unicode String
std::wstring utf8_decode(const string& str);
#endif
