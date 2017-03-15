#include <stdafx.h>
#include "log.h"

namespace mgc {
	namespace log {
		Log::Log(SDL_LogPriority priority, string name) : name(name), priority(priority) {};

		Log::Log(Log& log) {
			priority = log.priority;
			name = log.name;
		};

		int Log::sync() {
			if (name.empty())
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, priority, this->str().c_str());
			else SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, priority, "[%s] %s", name.c_str(), this->str().c_str());

			return 0;
		}

		LogStream::LogStream(Log _log) : std::ostream(&log), log(_log) {};
		LogStream::LogStream(LogStream&& other) : std::ostream(std::move(other)), log(other.log) {
			this->set_rdbuf(&log);
		}

		LogStream LogStream::operator() (const string& name) {
			return LogStream { Log(this->log.priority, name) };
		}

		LogStream critical { Log(SDL_LOG_PRIORITY_CRITICAL) };
		LogStream error { Log(SDL_LOG_PRIORITY_ERROR) };
		LogStream warn { Log(SDL_LOG_PRIORITY_WARN) };
		LogStream info { Log(SDL_LOG_PRIORITY_INFO) };
		LogStream debug { Log(SDL_LOG_PRIORITY_DEBUG) };
	}
}