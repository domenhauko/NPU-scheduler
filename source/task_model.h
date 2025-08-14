/*
 * task_model.h
 *
 *  Created on: 18 Jun 2025
 *      Author: Domen
 *
 * 	This is a struct of our proposed Task model.
 */

#ifndef TASK_MODEL_H
#define TASK_MODEL_H

#include <stdint.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "task.h"

typedef enum {
	TASK_TYPE_CPU,
	TASK_TYPE_NPU
} TaskType;

typedef enum {
	DEADLINE_HARD,		// Must never miss - system failure if missed
	DEADLINE_SOFT		// Value decreases if missed, but not catastrophic
} DeadlineType;

typedef enum {
	TASK_STATE_READY,		// Task is ready to run
	TASK_STATE_RUNNING,		// Task is currently executing
	TASK_STATE_PREEMPTED,	// Task was preempted and needs to restart
	TASK_STATE_WAITING		// Task is waiting for its release time
} TaskState;

typedef struct TaskModel {
	uint32_t releaseTime;		// R: Release time in microseconds
	uint32_t period;			// PI: Period in microseconds
	uint32_t deadline; 			// D: Deadline in microseconds (relative to release time)
	uint32_t capacity;			// C: Computation time (WCET)
	uint32_t priority;			// P: Task priority (lower number = higher priority)
	uint32_t stackSize;			// S: Stack size
	TaskType type;				// T: CPU or NPU
	DeadlineType deadlineType;	// Hard or soft deadline
	TaskState state;			// Current task state
	uint32_t absoluteDeadline;	// Absolute deadline (releaseTime + deadline)
	const char *name;			// Optional: Task name for logging
	void (*taskFunc)(void *); 	// Pointer to task function
	struct TaskModel *next;		// N: Pointer to next task
	void *params;				// Optional: Task-specific parameters
	TaskHandle_t handle;		// FreeRTOS task handle
} TaskModel;

#endif //TASK_MODEL_H
