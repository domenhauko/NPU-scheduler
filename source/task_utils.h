/*
 * task_utils.h
 *
 *  Created on: 24 Jul 2025
 *      Author: Domen
 */

#ifndef TASK_UTILS_H_
#define TASK_UTILS_H_

#include "task_model.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdint.h>

#define MAX_TASKS 10
#define MAX_INTERVALS 20  // Maximum execution intervals to track per task

// Structure to track task execution statistics
typedef struct {
    char taskName[32];          // Task name for identification
    uint32_t executionCount;    // Number of times this task has executed
    uint32_t startTimes[MAX_INTERVALS];  // Array of start times
    uint32_t endTimes[MAX_INTERVALS];    // Array of end times
    int isActive;               // Whether this slot is being used
} TaskExecutionTracker;

#ifdef __cplusplus
extern "C" {
#endif

// Duration calculation
uint32_t calculateDuration(uint32_t startTime, uint32_t endTime);

// Task creation
BaseType_t createCustomTask(TaskModel *task);

// Generic timing tracking functions
void initTaskTracking(void);
void addTaskStartTime(TaskModel *task, uint32_t time);
void addTaskEndTime(TaskModel *task, uint32_t time);
void printAllTaskDurations(void);
void printTaskExecutionStats(const char* taskName);
void clearTaskTrackingData(void);

// Task information functions
void printTaskName(TaskModel* task);
void printTask(TaskModel* task);
void printTaskType(TaskModel* task);

// Task list management functions
void printTaskList(void);
void printTaskListNames(void);
void addTaskToList(TaskModel* task);
void removeTaskFromList(TaskModel* task);
int getTaskCount(void);
TaskModel* getTaskListElement(int i);
TaskModel* getTaskByIndex(int index);
TaskModel* getTaskByName(const char* name);
TaskModel* isTaskHandleNull(TaskModel* task);

#ifdef __cplusplus
}
#endif

#endif /* TASK_UTILS_H_ */
