/*
 * task_utils.c
 *
 *  Created on: 24 Jul 2025
 *      Author: Domen
 */

#include <string.h>

#include "fsl_debug_console.h"
#include "timer.h"

#include "FreeRTOS.h"
#include "task.h"
#include "task_model.h"
#include "task_utils.h"

// Static storage for task management
static TaskModel* taskList[MAX_TASKS];
static int taskCount = 0;

// Generic task execution tracking
static TaskExecutionTracker taskTrackers[MAX_TASKS];
static int trackerCount = 0;

// Initialize task tracking system
void initTaskTracking(void) {
    trackerCount = 0;
    for (int i = 0; i < MAX_TASKS; i++) {
        taskTrackers[i].isActive = 0;
        taskTrackers[i].executionCount = 0;
        memset(taskTrackers[i].taskName, 0, sizeof(taskTrackers[i].taskName));
        memset(taskTrackers[i].startTimes, 0, sizeof(taskTrackers[i].startTimes));
        memset(taskTrackers[i].endTimes, 0, sizeof(taskTrackers[i].endTimes));
    }
    PRINTF("[TASK_UTILS] Task tracking initialized\r\n");
}

// Find or create a tracker for a given task
static TaskExecutionTracker* getOrCreateTracker(const char* taskName) {
    // First, try to find existing tracker
    for (int i = 0; i < MAX_TASKS; i++) {
        if (taskTrackers[i].isActive &&
            strcmp(taskTrackers[i].taskName, taskName) == 0) {
            return &taskTrackers[i];
        }
    }

    // If not found, create new tracker
    if (trackerCount < MAX_TASKS) {
        TaskExecutionTracker* tracker = &taskTrackers[trackerCount];
        tracker->isActive = 1;
        tracker->executionCount = 0;
        strncpy(tracker->taskName, taskName, sizeof(tracker->taskName) - 1);
        tracker->taskName[sizeof(tracker->taskName) - 1] = '\0'; // Ensure null termination
        trackerCount++;
        //PRINTF("[TASK_UTILS] Created new tracker for task: %s\r\n", taskName);
        return tracker;
    }

    PRINTF("[TASK_UTILS] Warning: Cannot create tracker for %s, max trackers reached\r\n", taskName);
    return NULL;
}

// Safe duration calculation that handles wraparound
uint32_t calculateDuration(uint32_t startTime, uint32_t endTime) {
    return TIMER_CalculateDuration(startTime, endTime);
}

// Create a custom task and add it to the task list
BaseType_t createCustomTask(TaskModel *task) {
    BaseType_t result = xTaskCreate(
        task->taskFunc,
        task->name,
        task->stackSize,
        (void *) task,
        task->priority,
        &(task->handle)  // Store the handle in TaskModel
    );

    if (result == pdPASS) {
        addTaskToList(task);
        PRINTF("[TASK_UTILS] Task created: %s (Handle: %p)\r\n", task->name, task->handle);
    } else {
        PRINTF("[TASK_UTILS] Failed to create task: %s\r\n", task->name);
    }

    return result;
}

// Add start time for any task
void addTaskStartTime(TaskModel *task, uint32_t time) {
    if (task == NULL || task->name == NULL) {
        return;
    }

    TaskExecutionTracker* tracker = getOrCreateTracker(task->name);
    if (tracker == NULL) {
        return;
    }

    // Add start time to the current execution slot
    uint32_t execIndex = tracker->executionCount % MAX_INTERVALS;
    tracker->startTimes[execIndex] = time;
}

// Add end time for any task and increment execution count
void addTaskEndTime(TaskModel *task, uint32_t time) {
    if (task == NULL || task->name == NULL) {
        return;
    }

    TaskExecutionTracker* tracker = getOrCreateTracker(task->name);
    if (tracker == NULL) {
        return;
    }

    // Add end time to the current execution slot
    uint32_t execIndex = tracker->executionCount % MAX_INTERVALS;
    tracker->endTimes[execIndex] = time;

    // Increment execution count after recording the end time
    tracker->executionCount++;
}

// Print execution statistics for a specific task
void printTaskExecutionStats(const char* taskName) {
    TaskExecutionTracker* tracker = NULL;

    // Find the tracker for this task
    for (int i = 0; i < MAX_TASKS; i++) {
        if (taskTrackers[i].isActive &&
            strcmp(taskTrackers[i].taskName, taskName) == 0) {
            tracker = &taskTrackers[i];
            break;
        }
    }

    if (tracker == NULL) {
        PRINTF("[TASK_UTILS] No tracking data found for task: %s\r\n", taskName);
        return;
    }

    PRINTF("=== Task Execution Stats: %s ===\r\n", taskName);
    PRINTF("Total executions: %u\r\n", tracker->executionCount);

    uint32_t intervals = (tracker->executionCount < MAX_INTERVALS) ?
                         tracker->executionCount : MAX_INTERVALS;

    if (intervals > 0) {
        PRINTF("Recent execution intervals:\r\n");
        for (uint32_t i = 0; i < intervals; i++) {
            uint32_t duration = calculateDuration(tracker->startTimes[i], tracker->endTimes[i]);
            PRINTF("  Execution %u: Start=%u, End=%u, Duration=%u us\r\n",
                   i + 1, tracker->startTimes[i], tracker->endTimes[i], duration);
        }
    }
    PRINTF("================================\r\n");
}

// Print duration data for all tracked tasks in a format suitable for analysis
void printAllTaskDurations(void) {
    PRINTF("\r\n=== ALL TASK EXECUTION DURATIONS ===\r\n");

    for (int i = 0; i < trackerCount; i++) {
        TaskExecutionTracker* tracker = &taskTrackers[i];
        if (!tracker->isActive) {
            continue;
        }

        PRINTF("'%s': [\r\n", tracker->taskName);

        uint32_t intervals = (tracker->executionCount < MAX_INTERVALS) ?
                             tracker->executionCount : MAX_INTERVALS;

        for (uint32_t j = 0; j < intervals; j++) {
            PRINTF("    (%u, %u)", tracker->startTimes[j], tracker->endTimes[j]);

            if (j < intervals - 1) {
                PRINTF(",\r\n");
            } else {
                PRINTF("\r\n");
            }
        }

        PRINTF("],\r\n");
    }
    PRINTF("===================================\r\n\r\n");
}

// Clear all tracking data
void clearTaskTrackingData(void) {
    PRINTF("[TASK_UTILS] Clearing all task tracking data\r\n");
    initTaskTracking();
}

// Print basic task information
void printTaskName(TaskModel* task) {
    if (task && task->name) {
        PRINTF("Task: %s\r\n", task->name);
    }
}

void printTask(TaskModel* task) {
    if (task == NULL) {
        PRINTF("Task: NULL\r\n");
        return;
    }

    PRINTF("Task: %s (Handle: %p)\r\n", task->name ? task->name : "Unknown", task->handle);
    PRINTF("[%s] ReleaseTime: %u\r\n", task->name ? task->name : "Unknown", task->releaseTime);
    PRINTF("[%s] Period: %u\r\n", task->name ? task->name : "Unknown", task->period);
    PRINTF("[%s] Deadline: %u\r\n", task->name ? task->name : "Unknown", task->deadline);
    PRINTF("[%s] Capacity: %u\r\n", task->name ? task->name : "Unknown", task->capacity);
    PRINTF("[%s] Priority: %u\r\n", task->name ? task->name : "Unknown", task->priority);
    PRINTF("[%s] StackSize: %u\r\n", task->name ? task->name : "Unknown", task->stackSize);
    printTaskType(task);
}

void printTaskType(TaskModel* task) {
    if (task == NULL || task->name == NULL) {
        return;
    }

    if (task->type == TASK_TYPE_CPU) {
        PRINTF("[%s] TaskType: CPU\r\n", task->name);
    } else if (task->type == TASK_TYPE_NPU) {
        PRINTF("[%s] TaskType: NPU\r\n", task->name);
    } else {
        PRINTF("[%s] TaskType: Unknown (%d)\r\n", task->name, task->type);
    }
}

// Task list management functions
void printTaskList(void) {
    PRINTF("Current task list (%d tasks):\r\n", taskCount);
    if (taskCount == 0) {
        PRINTF("Currently no tasks in taskList.\r\n");
    } else {
        for (int i = 0; i < taskCount; i++) {
            PRINTF("  [%d] ", i);
            printTask(taskList[i]);
            PRINTF("\r\n");
        }
    }
}

void printTaskListNames(void) {
    PRINTF("Current task list names (%d tasks):\r\n", taskCount);
    if (taskCount == 0) {
        PRINTF("Currently no tasks in taskList.\r\n");
    } else {
        for (int i = 0; i < taskCount; i++) {
            PRINTF("  [%d] %s\r\n", i, taskList[i]->name ? taskList[i]->name : "Unknown");
        }
    }
}

void addTaskToList(TaskModel* task) {
    if (task == NULL) {
        PRINTF("[TASK_UTILS] Cannot add NULL task to list\r\n");
        return;
    }

    if (taskCount < MAX_TASKS) {
        taskList[taskCount++] = task;
        PRINTF("[TASK_UTILS] Added task %s to list (position %d)\r\n",
               task->name ? task->name : "Unknown", taskCount - 1);
    } else {
        PRINTF("[TASK_UTILS] Task list full. Cannot add task: %s\r\n",
               task->name ? task->name : "Unknown");
    }
}

void removeTaskFromList(TaskModel* task) {
    if (task == NULL) {
        return;
    }

    for (int i = 0; i < taskCount; i++) {
        if (taskList[i] == task) {
            // Shift remaining tasks down
            for (int j = i; j < taskCount - 1; j++) {
                taskList[j] = taskList[j + 1];
            }
            taskList[taskCount - 1] = NULL;
            taskCount--;
            PRINTF("[TASK_UTILS] Removed task %s from list\r\n",
                   task->name ? task->name : "Unknown");
            break;
        }
    }
}

// Get current task count
int getTaskCount(void) {
    return taskCount;
}

TaskModel* getTaskListElement(int i) {
	return taskList[i];
}

// Get task by index
TaskModel* getTaskByIndex(int index) {
    if (index >= 0 && index < taskCount) {
        return taskList[index];
    }
    return NULL;
}

TaskModel* isTaskHandleNull(TaskModel* task) {
	if (task == NULL) {
	        return NULL;
	    }

	if (task->handle == NULL) {
		PRINTF("[TASK UTIL] Task handle for %s is NULL\r\n", task->name ? task->name : "(unnamed)");
	} else {
		PRINTF("[TASK UTIL] Task handle for %s is not NULL: %p\r\n",
			   task->name ? task->name : "(unnamed)", (void *)task->handle);
	}
	return task;
}

// Get task by name
TaskModel* getTaskByName(const char* name) {
    if (name == NULL) {
        return NULL;
    }

    for (int i = 0; i < taskCount; i++) {
        if (taskList[i] && taskList[i]->name &&
            strcmp(taskList[i]->name, name) == 0) {
            return taskList[i];
        }
    }
    return NULL;
}
