/*
#include <exampleTwo_tasks.h>
 * exampleOne_tasks.c
 *
 *  Created on: 24 Jul 2025
 *      Author: Domen
 */

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <task_utils.h>

#include "fsl_debug_console.h"
#include "image.h"
#include "image_utils.h"
#include "model.h"
#include "output_postproc.h"
#include "timer.h"
#include "perf_timer.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "task_model.h"
#include "scheduler.h"
#include "random_task_generator.h"
#include "exampleOne_tasks.h"

// Example of how to update your task creation in exampleOne_tasks.c

void createExampleOneTasks(void) {
    static TaskModel task1;
    static TaskModel task2;
    static TaskModel task3;
    static TaskModel task4;
    static TaskModel task5;

    // Initialize tasks with new fields
    task1 = (TaskModel){
        .releaseTime = 100,
        .period = 500000,
        .deadline = 500000,
        .capacity = 150000,
        .priority = TASK1_TASK_PRIORITY,
        .stackSize = TASK1_TASK_STACK_SIZE,
        .type = TASK_TYPE_CPU,
        .deadlineType = DEADLINE_HARD,        // NEW: Set as hard deadline task
        .state = TASK_STATE_WAITING,          // NEW: Initial state
        .absoluteDeadline = 0,                // NEW: Will be calculated by scheduler
        .name = "Task1",
        .taskFunc = taskFunction,
        .next = &task2,
        .params = NULL,
        .handle = NULL
    };

    task2 = (TaskModel){
        .releaseTime = 100,
        .period = 500000,
        .deadline = 500000,
        .capacity = 100000,
        .priority = TASK2_TASK_PRIORITY,
        .stackSize = TASK2_TASK_STACK_SIZE,
        .type = TASK_TYPE_NPU,
        .deadlineType = DEADLINE_HARD,        // NEW
        .state = TASK_STATE_WAITING,          // NEW
        .absoluteDeadline = 0,                // NEW
        .name = "Task2",
        .taskFunc = taskFunction,
        .next = &task3,
        .params = NULL,
        .handle = NULL
    };

    task3 = (TaskModel){
        .releaseTime = 100,
        .period = 500000,
        .deadline = 500000,
        .capacity = 150000,
        .priority = TASK3_TASK_PRIORITY,
        .stackSize = TASK3_TASK_STACK_SIZE,
        .type = TASK_TYPE_CPU,
        .deadlineType = DEADLINE_SOFT,        // NEW: Set as soft deadline task
        .state = TASK_STATE_WAITING,          // NEW
        .absoluteDeadline = 0,                // NEW
        .name = "Task3",
        .taskFunc = taskFunction,
        .next = NULL,
        .params = NULL,
        .handle = NULL
    };

    task4 = (TaskModel){
        .releaseTime = 0,
        .period = 50000,
        .deadline = 50000,
        .capacity = 20000,
        .priority = TASK4_TASK_PRIORITY,
        .stackSize = TASK4_TASK_STACK_SIZE,
        .type = TASK_TYPE_CPU,
        .deadlineType = DEADLINE_HARD,        // NEW
        .state = TASK_STATE_WAITING,          // NEW
        .absoluteDeadline = 0,                // NEW
        .name = "Task4",
        .taskFunc = taskFunction,
        .next = NULL,
        .params = NULL,
        .handle = NULL
    };

    task5 = (TaskModel){
        .releaseTime = 0,
        .period = 75000,
        .deadline = 75000,
        .capacity = 50000,
        .priority = TASK5_TASK_PRIORITY,
        .stackSize = TASK5_TASK_STACK_SIZE,
        .type = TASK_TYPE_CPU,
        .deadlineType = DEADLINE_SOFT,        // NEW
        .state = TASK_STATE_WAITING,          // NEW
        .absoluteDeadline = 0,                // NEW
        .name = "Task5",
        .taskFunc = taskFunction,
        .next = NULL,
        .params = NULL,
        .handle = NULL
    };

    createCustomTask(&task1);
    createCustomTask(&task2);
    createCustomTask(&task3);
    createCustomTask(&task4);
    createCustomTask(&task5);
}

void taskFunction(void *pvParameters)
{
    TaskModel *task = (TaskModel *)pvParameters;
    uint32_t workStartTime;

    while (1) {
        // Wait for the scheduler to notify this task to run.
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Accurately simulate work by busy-waiting for the specified capacity in microseconds.
        workStartTime = TIMER_GetTimeInUS();
        while (calculateDuration(workStartTime, TIMER_GetTimeInUS()) < task->capacity) {
            // This loop consumes CPU cycles to simulate work.
            // A 'nop' (no operation) is used to prevent the compiler from optimizing the loop away.
            __asm volatile("nop");
        }

        PRINTF("[TASK] %s: Finished work.\r\n", task->name);

        // Signal completion back to the scheduler for the correct resource.
        if (task->type == TASK_TYPE_CPU) {
            setCpuBusy(0);
        } else if (task->type == TASK_TYPE_NPU) {
            setNpuBusy(0);
        }
    }
}

