/*
* random_task_generator.c
*/

#include "random_task_generator.h"
#include "scheduler.h"
#include "fsl_debug_console.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_RANDOM_TASKS 20
#define MAX_TASK_NAME_LENGTH 32

// Static storage for random tasks
static TaskModel randomTasks[MAX_RANDOM_TASKS];
static char taskNames[MAX_RANDOM_TASKS][MAX_TASK_NAME_LENGTH];
static uint32_t randomDelays[MAX_RANDOM_TASKS]; // Store delay for each task
static int randomTaskCount = 0;

// Simple PRNG state
static uint32_t rngState = 12345;


// Default configuration with reasonable ranges
const RandomTaskConfig DEFAULT_RANDOM_CONFIG = {
	.releaseTimeMin = 0,      // 100 μs
	.releaseTimeMax = 10000,    // 10 ms
	.periodMin = 200000,        // 200 ms
	.periodMax = 1000000,       // 1 second
	.capacityMin = 20000,       // 20 ms
	.capacityMax = 150000,      // 150 ms
	.priorityMin = 1,
	.priorityMax = 15,
	.stackSizeMin = 256,
	.stackSizeMax = 1024,
	.cpuTaskRatio = 0.7f        // 70% CPU tasks, 30% NPU tasks
};


void initRandomTaskGenerator(uint32_t seed) {
    rngState = seed;
    randomTaskCount = 0;
    PRINTF("[RANDOM_TASK_GEN] Initialized with seed: %u\r\n", seed);
}

// Simple linear congruential generator
static uint32_t nextRandom(void) {
    rngState = (rngState * 1103515245 + 12345) & 0x7fffffff;
    return rngState;
}

uint32_t getRandomNumber(uint32_t min, uint32_t max) {
    if (min >= max) return min;
    return min + (nextRandom() % (max - min + 1));
}

TaskModel* generateRandomTask(const char* baseName, int taskId, const RandomTaskConfig* config) {
    if (randomTaskCount >= MAX_RANDOM_TASKS) {
        PRINTF("[RANDOM_TASK_GEN] Maximum random tasks reached\r\n");
        return NULL;
    }

    TaskModel* task = &randomTasks[randomTaskCount];
    char* taskName = taskNames[randomTaskCount];

    // Generate task name
    snprintf(taskName, MAX_TASK_NAME_LENGTH, "%s_%d", baseName, taskId);

    // Generate random parameters
    uint32_t releaseTime = getRandomNumber(config->releaseTimeMin, config->releaseTimeMax);
    uint32_t period = getRandomNumber(config->periodMin, config->periodMax);
    uint32_t capacity = getRandomNumber(config->capacityMin, config->capacityMax);

    // Ensure capacity doesn't exceed period
    if (capacity > period) {
        capacity = period / 2; // Set to half the period as a safe value
    }

    uint32_t priority = getRandomNumber(config->priorityMin, config->priorityMax);
    uint32_t stackSize = getRandomNumber(config->stackSizeMin, config->stackSizeMax);

    // Determine task type based on ratio
    TaskType taskType = (nextRandom() / (float)0x7fffffff) < config->cpuTaskRatio ?
                        TASK_TYPE_CPU : TASK_TYPE_NPU;

    // Store random delay (in ms) for the task function
    randomDelays[randomTaskCount] = capacity; // Convert to ms

    // Initialize the task model
    *task = (TaskModel){
        .releaseTime = releaseTime,
        .period = period,
        .deadline = period, // Deadline equals period
        .capacity = capacity,
        .priority = priority,
        .stackSize = stackSize,
        .type = taskType,
        .name = taskName,
        .taskFunc = randomTask,
        .next = NULL,
        .params = &randomDelays[randomTaskCount], // Pass delay as parameter
        .handle = NULL
    };

    randomTaskCount++;

    PRINTF("[RANDOM_TASK_GEN] Generated task: %s\r\n", taskName);
    PRINTF("  Release: %u us, Period: %u us, Capacity: %u us\r\n",
           releaseTime, period, capacity);
    PRINTF("  Priority: %u, Stack: %u, Type: %s, Delay: %u us\r\n",
           priority, stackSize,
           taskType == TASK_TYPE_CPU ? "CPU" : "NPU",
           randomDelays[randomTaskCount - 1]);

    return task;
}

void randomTask(void *pvParameters) {
    TaskModel *task = (TaskModel *)pvParameters;
    uint32_t *taskDelay = (uint32_t *)task->params;

    while (1) {
        // Wait for scheduler notification
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Simulate work with the specified delay
        vTaskDelay(pdMS_TO_TICKS(taskDelay));

        PRINTF("[RANDOM_TASK] %s: Completed work (delay: %u ms)\r\n",
               task->name, taskDelay);

        // Signal completion back to scheduler
        if (task->type == TASK_TYPE_CPU) {
            setCpuBusy(0);
        } else {
            setNpuBusy(0);
        }
    }
}

void createRandomTasks(int numTasks, const RandomTaskConfig* config) {
    if (config == NULL) {
        config = &DEFAULT_RANDOM_CONFIG;
    }

    if (numTasks > MAX_RANDOM_TASKS) {
        PRINTF("[RANDOM_TASK_GEN] Requested %d tasks, limiting to %d\r\n",
               numTasks, MAX_RANDOM_TASKS);
        numTasks = MAX_RANDOM_TASKS;
    }

    PRINTF("[RANDOM_TASK_GEN] Creating %d random tasks...\r\n", numTasks);

    for (int i = 0; i < numTasks; i++) {
        TaskModel* task = generateRandomTask("RandomTask", i + 1, config);
        if (task == NULL) {
            PRINTF("[RANDOM_TASK_GEN] Failed to generate task %d\r\n", i + 1);
            break;
        }

        // Create the FreeRTOS task
        BaseType_t result = xTaskCreate(
			task->taskFunc,
			task->name,
			task->stackSize,
			(void *) task,
			task->priority,
			&(task->handle)  // Store the handle in TaskModel
		);

        PRINTF("This place is never reached.\r\n");

        if (result == pdPASS) {
            addTaskToList(task);
            PRINTF("[RANDOM_TASK_GEN] Created task: %s (Handle: %p)\r\n",
                   task->name, task->handle);
        } else {
            PRINTF("[RANDOM_TASK_GEN] Failed to create FreeRTOS task: %s\r\n",
                   task->name);
            randomTaskCount--; // Rollback the count
        }
    }

    PRINTF("[RANDOM_TASK_GEN] Successfully created %d random tasks\r\n",
           randomTaskCount);
}

void cleanupRandomTasks(void) {
    PRINTF("[RANDOM_TASK_GEN] Cleaning up %d random tasks...\r\n", randomTaskCount);

    for (int i = 0; i < randomTaskCount; i++) {
        TaskModel* task = &randomTasks[i];
        if (task->handle != NULL) {
            // Remove from scheduler's task list
            removeTaskFromList(task);

            // Delete the FreeRTOS task
            vTaskDelete(task->handle);
            task->handle = NULL;

            PRINTF("[RANDOM_TASK_GEN] Deleted task: %s\r\n", task->name);
        }
    }

    randomTaskCount = 0;
    PRINTF("[RANDOM_TASK_GEN] Cleanup complete\r\n");
}

