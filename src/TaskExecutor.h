#pragma once

#include <stdafx.h>

#include <queue>
using std::queue;

#include "Task.h"

namespace mgc {
	
	queue<Task*> tasks_waiting{};
	list<Task*> tasks_active{};

	static void add_task(Task* task) {
		tasks_waiting.push(task);
	}

	static void activate_tasks() {
		while (!tasks_waiting.empty()) {
			Task* task = tasks_waiting.front();
			tasks_active.push_back(task);
			tasks_waiting.pop();
		}
	}

	static void process_tasks(int time) {
		auto& task = tasks_active.begin();

		while (task != tasks_active.end()) {
			Task* task_process = *task;
			task++;
			task_process->update(time);
			if (task_process->finished) {
				tasks_active.remove(task_process);
				delete task_process;
			}
		}
	}

	static void update_tasks(int time) {
		activate_tasks();
		process_tasks(time);
	}
}