/*
#include <exampleTwo_tasks.h>
#include <exampleOneReal_tasks.h>
 * Copyright 2020-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "board_init.h"
#include "demo_config.h"
#include "demo_info.h"
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
#include "mobilenet_tasks.h"
#include "scheduler.h"
#include "random_task_generator.h"


#include "exampleOne_tasks.h"
#include "exampleTwo_tasks.h"
#include "exampleThree_tasks.h"

// Add this define to switch between hardcoded and random tasks
#define USE_RANDOM_TASKS 0  // Set to 0 for hardcoded tasks, 1 for random tasks
#define USE_MOBILENET_TASKS 1	// Set to 0 for other examples, 1 for MobileNet example
#define USE_EXAMPLEONE_TASKS 0	// Set to 0 for other examples, 1 for ExampleOne example
#define USE_EXAMPLETWO_TASKS 0	// Set to 0 for other examples, 1 for ExampleTwo example
#define USE_EXAMPLETHREE_TASKS 0	// Set to 0 for other examples, 1 for ExampleThree example
#define NUM_RANDOM_TASKS 3 // Number of random tasks to generate

/* Prototypes */
BaseType_t createCustomTask(TaskModel *task);


int main(void)
{
    BOARD_Init();
    TIMER_Init();

    PRINTF("------------------\r\n");
    PRINTF("Starting Custom Task Scheduler\r\n");

#if USE_RANDOM_TASKS
    // Initialize random task generator with a seed
    // You can use a timer value or any other source for the seed
    uint32_t seed = TIMER_GetTimeInUS();
    initRandomTaskGenerator(seed);

    // Optional: Create custom configuration for random tasks
    RandomTaskConfig customConfig = {
        .releaseTimeMin = 100,      // 100 μs
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

    // Create random tasks (pass NULL to use default config)
    createRandomTasks(NUM_RANDOM_TASKS, &customConfig);

    PRINTF("Generated %d random tasks\r\n", NUM_RANDOM_TASKS);
#endif

#if USE_MOBILENET_TASKS
    // Create mobilenet tasks for testing
    createMobileNetTasks();
    PRINTF("Created MobileNet test tasks\r\n");
#endif

#if USE_EXAMPLEONE_TASKS
    // Create exampleOne tasks for testing
    createExampleOneTasks();
    PRINTF("Created ExampleOne test tasks\r\n");
#endif

#if USE_EXAMPLETWO_TASKS
    // Create exampleTwo tasks for testing
    createSchedulerDemoTasks();
    PRINTF("Created ExampleTwo test tasks\r\n");
#endif

#if USE_EXAMPLETHREE_TASKS
    // Create exampleThree tasks for testing
    createExampleThreeTasks();
    PRINTF("Created ExampleThree test tasks\r\n");
#endif

    // Create the task scheduler
    xTaskCreate(runTaskScheduler, "TaskScheduler", 1024, NULL, 0, NULL);

    vTaskStartScheduler();

    // Should never reach here
    for(;;);
}



void vApplicationMallocFailedHook(void)
{
    PRINTF("Malloc failed!\r\n");
    for (;;);
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    PRINTF("Stack overflow in task: %s\r\n", pcTaskName);
    for (;;);
}
