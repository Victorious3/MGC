#include <stdafx.h>

#include "InterpolateTask.h"
#include "render.h"

namespace mgc{
	InterpolateTask::InterpolateTask(float& target, int start_val, int end_val, int start_time, int end_time) : 
		target(target),
		start_val(start_val), end_val(end_val),
		start_time(start_time), end_time(end_time) { }

	InterpolateTask::~InterpolateTask() {

	}

	void InterpolateTask::finish() {
		finished = true;
		target = end_val;
	}

	void InterpolateTask::update(int time) {
		if (finished) return;

		if (time > end_time) {
			finish();
			return;
		}

		render::linear_interpolatef(target, start_time, end_time, start_val, end_val);
	}
}