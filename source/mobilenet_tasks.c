/*
 * mobilenet_tasks.c
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
#include "mobilenet_tasks.h"
#include "scheduler.h"

void createMobileNetTasks(void) {
    static TaskModel hello;
    static TaskModel mobile;
    static TaskModel goodbye;
    static TaskModel taskD_EDF;

    // Initialize tasks
    goodbye = (TaskModel){
    	.name = "Goodbye",
    	.taskFunc = goodbyeTask,
        .priority = GOODBYE_TASK_PRIORITY,
        .type = TASK_TYPE_CPU,
		.deadlineType = DEADLINE_SOFT,
        .releaseTime = 0,
        .period = 250000,
        .deadline = 250000,
        .capacity = 50000,
        .stackSize = GOODBYE_TASK_STACK_SIZE,
		.state = TASK_STATE_WAITING,
        .params = NULL,
        .handle = NULL
    };

    mobile = (TaskModel){
    	.name = "MobileNet",
    	.taskFunc = mobilenetTask,
        .priority = MOBILENET_TASK_PRIORITY,
        .type = TASK_TYPE_NPU,
		.deadlineType = DEADLINE_HARD,
        .releaseTime = 0,
        .period = 100000,
        .deadline = 100000,
        .capacity = 40000,
        .stackSize = MOBILENET_TASK_STACK_SIZE,
		.state = TASK_STATE_WAITING,
        .params = NULL,
        .handle = NULL
    };

    hello = (TaskModel){
    	.name = "Hello",
    	.taskFunc = helloTask,
        .priority = HELLO_TASK_PRIORITY,
        .type = TASK_TYPE_CPU,
		.deadlineType = DEADLINE_HARD,
        .releaseTime = 30000,
        .period = 100000,       // 100 ms
        .deadline = 100000,     // 100 ms (tight deadline to force preemption)
        .capacity = 30000,      // 30 ms of work
        .stackSize = HELLO_TASK_STACK_SIZE,
		.state = TASK_STATE_WAITING,
        .params = NULL,
        .handle = NULL
    };

    // Task D: A high-priority task released at the same time as Task C
	// to demonstrate Earliest Deadline First (EDF) tie-breaking.
	// It has the same priority as Task C, but a later deadline, so Task C will be chosen first.
	taskD_EDF = (TaskModel){
		.name = "CPU_Hard_EDF_Tie",
		.taskFunc = taskFunctionEDF,
		.priority = HELLO_TASK_PRIORITY,
		.type = TASK_TYPE_CPU,
		.deadlineType = DEADLINE_HARD,
		.releaseTime = 30000,   // Released at the same time as Task C
		.period = 250000,       // 250 ms
		.deadline = 250000,      // 250 ms (later deadline than Task C)
		.capacity = 40000,      // 40 ms of work
		.stackSize = DEMO_STACK_SIZE,
		.state = TASK_STATE_WAITING,
		.handle = NULL,
	};

    createCustomTask(&hello);
    createCustomTask(&mobile);
    createCustomTask(&goodbye);
    createCustomTask(&taskD_EDF);
}

void mobilenetTask(void *pvParameters)
{
    TaskModel *task = (TaskModel *)pvParameters;

    // Initialize model once
    if (MODEL_Init() != kStatus_Success) {
        PRINTF("[%s] Failed initializing model\r\n", task->name);
        vTaskSuspend(NULL);
    }

    tensor_dims_t inputDims;
    tensor_type_t inputType;
    uint8_t *inputData = MODEL_GetInputTensorData(&inputDims, &inputType);

    tensor_dims_t outputDims;
    tensor_type_t outputType;
    uint8_t *outputData = MODEL_GetOutputTensorData(&outputDims, &outputType);

    while (1) {
        // Wait for scheduler notification
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        /* Expected tensor dimensions: [batches, height, width, channels] */
        if (IMAGE_GetImage(inputData, inputDims.data[2], inputDims.data[1], inputDims.data[3]) != kStatus_Success) {
            PRINTF("[%s] Failed retrieving input image\r\n", task->name);
            setNpuBusy(0);
            continue;
        }

        MODEL_ConvertInput(inputData, &inputDims, inputType);
        uint32_t startTime = TIMER_GetTimeInUS();
        MODEL_RunInference();
        uint32_t endTime = TIMER_GetTimeInUS();

        uint32_t duration = calculateDuration(startTime, endTime);
        PRINTF("[NPU]: Duration = %u us (DWT-safe)\r\n", duration);

        //MODEL_ProcessOutput(outputData, &outputDims, outputType, duration);

        // Signal completion back to scheduler
        setNpuBusy(0);
    }
}

void helloTask(void *pvParameters)
{
    TaskModel *task = (TaskModel *)pvParameters;

    while (1) {
        // Wait for scheduler notification
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Simulate work
        vTaskDelay(pdMS_TO_TICKS(task->capacity));
        //PRINTF("[TASK] %s: Hello world!\r\n", task->name);

        // Signal completion back to scheduler
        setCpuBusy(0);
    }
}

void goodbyeTask(void *pvParameters)
{
    TaskModel *task = (TaskModel *)pvParameters;

    while (1) {
        // Wait for scheduler notification
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Simulate work
        vTaskDelay(pdMS_TO_TICKS(task->capacity));
        //PRINTF("[TASK] %s: Goodbye!\r\n", task->name);

        // Signal completion back to scheduler
        setCpuBusy(0);
    }
}

void taskFunctionEDF(void *pvParameters)
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
        }
    }
}
