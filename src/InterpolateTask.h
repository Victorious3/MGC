#pragma once

#include <stdafx.h>

#include "Task.h"

namespace mgc {
	template <class T>
	class InterpolateTask : public Task {
	public:

		InterpolateTask(T& target, int start_val, int end_val, int start_time, int end_time);
		~InterpolateTask();

		void update(int time);
		void finish();

		T& target;
		int start_val;
		int end_val;
		int start_time;
		int end_time;
	};

	template <class T>
	InterpolateTask<T>::InterpolateTask(T& target, int start_val, int end_val, int start_time, int end_time) :
		target(target),
		start_val(start_val), end_val(end_val),
		start_time(start_time), end_time(end_time) { }

	template <class T>
	InterpolateTask<T>::~InterpolateTask() {

	}

	template <class T>
	void InterpolateTask<T>::finish() {
		finished = true;
		target = end_val;
	}

	template <class T>
	void InterpolateTask<T>::update(int time) {
		if (finished) return;

		if (time > end_time) {
			finish();
			return;
		}

		render::linear_interpolate<T>(target, start_time, end_time, start_val, end_val);
	}
}