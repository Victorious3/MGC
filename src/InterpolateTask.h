#pragma once

#include <stdafx.h>

#include "Task.h"

namespace mgc {
	class InterpolateTask : public Task {
	public:

		InterpolateTask(float& target, int start_val, int end_val, int start_time, int end_time);
		~InterpolateTask();

		void update(int time);
		void finish();

		float& target;
		int start_val;
		int end_val;
		int start_time;
		int end_time;
	};
}