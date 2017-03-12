#pragma once
#include "stddef.h"

#include <sstream>

namespace mgc {
	namespace log {
		class LogStream;
		class Log : public std::stringbuf {
		public:
			Log(SDL_LogPriority priority, string name = "");
			Log(Log& log);

			virtual int sync() override;

		private:
			friend LogStream;
			string name;
			SDL_LogPriority priority;
		};

		class LogStream : public std::ostream {
		public:
			explicit LogStream(Log log);
			LogStream(LogStream&& other);
			LogStream operator() (const string& name);

			Log log;
		};

		extern LogStream critical;
		extern LogStream error;
		extern LogStream warn;
		extern LogStream info;
		extern LogStream debug;
	}
}
