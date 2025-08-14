/*
 * exampleTwo_tasks.c
 *
 *  Created on: 25 Jul 2025
 *      Author: Domen
 */

#include <stdio.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "task_model.h"
#include "task_utils.h"
#include "scheduler.h"
#include "timer.h" // Required for accurate microsecond delays
#include "fsl_debug_console.h"

// --- Task Priority Definitions ---
// Lower number means higher priority
#define HIGH_PRIORITY   3
#define MEDIUM_PRIORITY 2
#define LOW_PRIORITY    1

// --- Task Stack Size Definitions ---
#define DEMO_STACK_SIZE configMINIMAL_STACK_SIZE + 100

// --- Forward Declaration for the Task Function ---
void taskFunctionThree(void *pvParameters);

/**
 * @brief Creates and initializes a set of tasks to demonstrate the scheduler.
 *
 * This function defines 4 tasks:
 * 1. CPU_Soft_LowPrio: A long-running, low-priority soft-deadline task on the CPU.
 * 2. NPU_Hard_HighPrio: A high-priority, hard-deadline task for the NPU to show parallelism.
 * 3. CPU_Hard_HighPrio: A high-priority, hard-deadline CPU task that will preempt the low-priority task.
 * 4. CPU_Hard_EDF_Tie: A CPU task with the same high priority but a later deadline to show EDF tie-breaking.
 */
void createExampleThreeTasks(void) {
    static TaskModel taskA_CPUSoft;
    static TaskModel taskB_NPUHard;
    static TaskModel taskC_CPUHard;
    static TaskModel taskD_CPU_EDF;

    // Task A: A low-priority, soft-deadline task that will be preempted.
    taskA_CPUSoft = (TaskModel){
        .name = "CPU_Soft_LowPrio",
        .taskFunc = taskFunctionThree,
        .priority = LOW_PRIORITY,
        .type = TASK_TYPE_CPU,
        .deadlineType = DEADLINE_SOFT,
        .releaseTime = 0,
        .period = 250000,       // 250 ms
        .deadline = 250000,     // 250 ms
        .capacity = 50000,      // 50 ms of work
        .stackSize = DEMO_STACK_SIZE,
        .state = TASK_STATE_WAITING,
        .handle = NULL,
    };

    // Task B: A high-priority NPU task to show parallel resource usage.
    taskB_NPUHard = (TaskModel){
        .name = "NPU_Hard_HighPrio",
        .taskFunc = taskFunctionThree,
        .priority = HIGH_PRIORITY,
        .type = TASK_TYPE_NPU,
        .deadlineType = DEADLINE_HARD,
        .releaseTime = 0,
        .period = 200000,       // 200 ms
        .deadline = 200000,     // 200 ms
        .capacity = 100000,      // 100 ms of work
        .stackSize = DEMO_STACK_SIZE,
        .state = TASK_STATE_WAITING,
        .handle = NULL,
    };

    // Task C: A high-priority, hard-deadline task designed to preempt Task A.
    // Its deadline is short enough to trigger the "danger zone" preemption logic.
    taskC_CPUHard = (TaskModel){
        .name = "CPU_Hard_HighPrio",
        .taskFunc = taskFunctionThree,
        .priority = HIGH_PRIORITY,
        .type = TASK_TYPE_CPU,
        .deadlineType = DEADLINE_HARD,
        .releaseTime = 30000,   // Released 30ms after Task A starts
        .period = 100000,       // 100 ms
        .deadline = 100000,      // 100 ms (tight deadline to force preemption)
        .capacity = 40000,      // 40 ms of work
        .stackSize = DEMO_STACK_SIZE,
        .state = TASK_STATE_WAITING,
        .handle = NULL,
    };

    // Task D: A high-priority task released at the same time as Task C
    // to demonstrate Earliest Deadline First (EDF) tie-breaking.
    // It has the same priority as Task C, but a later deadline, so Task C will be chosen first.
    taskD_CPU_EDF = (TaskModel){
        .name = "CPU_Hard_EDF_Tie",
        .taskFunc = taskFunctionThree,
        .priority = HIGH_PRIORITY,
        .type = TASK_TYPE_CPU,
        .deadlineType = DEADLINE_HARD,
        .releaseTime = 30000,   // Released at the same time as Task C
        .period = 250000,       // 180 ms
        .deadline = 250000,      // 90 ms (later deadline than Task C)
        .capacity = 40000,      // 40 ms of work
        .stackSize = DEMO_STACK_SIZE,
        .state = TASK_STATE_WAITING,
        .handle = NULL,
    };

    // Register all tasks with the system
    createCustomTask(&taskA_CPUSoft);
    createCustomTask(&taskB_NPUHard);
    createCustomTask(&taskC_CPUHard);
    createCustomTask(&taskD_CPU_EDF);
}

void taskFunctionThree(void *pvParameters)
{
    TaskModel *task = (TaskModel *)pvParameters;
    //PRINTF("[TASK]The task %s started working.\r\n", task->name);
    uint32_t workStartTime;

    while (1) {
        // Wait for the scheduler to notify this task to run.
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        //vTaskDelay(task->capacity);


        // Accurately simulate work by busy-waiting for the specified capacity in microseconds.
        workStartTime = TIMER_GetTimeInUS();
        while (calculateDuration(workStartTime, TIMER_GetTimeInUS()) < task->capacity) {
            // This loop consumes CPU cycles to simulate work.
            // A 'nop' (no operation) is used to prevent the compiler from optimizing the loop away.
            __asm volatile("nop");
        }


        //PRINTF("[TASK] %s: Finished work.\r\n", task->name);

        // Signal completion back to the scheduler for the correct resource.
        if (task->type == TASK_TYPE_CPU) {
            setCpuBusy(0);
        } else if (task->type == TASK_TYPE_NPU) {
            setNpuBusy(0);
        }
    }
}

