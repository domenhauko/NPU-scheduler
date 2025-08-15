/*
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

/* Prototypes */
BaseType_t createCustomTask(TaskModel *task);


int main(void)
{
    BOARD_Init();
    TIMER_Init();

    PRINTF("------------------\r\n");
    PRINTF("Starting Custom Task Scheduler\r\n");

    // Create mobilenet tasks for testing
    createMobileNetTasks();
    PRINTF("Created MobileNet test tasks\r\n");

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
