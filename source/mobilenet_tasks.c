/*
 * mobilenet_tasks.c
 *
 *  Created on: 24 Jul 2025
 *      Author: Domen
 *
 *      custom_TaskModel_preempt
 */

#include <stdint.h>
#include "task_utils.h"

#include "fsl_debug_console.h"
#include "image.h"
#include "image_utils.h"
#include "model.h"
#include "timer.h"

#include "FreeRTOS.h"
#include "task.h"
#include "task_model.h"
#include "mobilenet_tasks.h"
#include "scheduler.h"

void createMobileNetTasks(void) {
    static TaskModel hello;
    static TaskModel mobile;
    //static TaskModel goodbye;
    //static TaskModel taskD_EDF;

    // Initialize tasks
    /*
    goodbye = (TaskModel){
    	.name = "Goodbye",
    	.taskFunc = goodbyeTask,
        .priority = 3,
        .type = TASK_TYPE_CPU,
		.deadlineType = DEADLINE_SOFT,
        .releaseTime = 0,
        .period = 1000000,
        .deadline = 1000000,
        .capacity = 500000,
        .stackSize = GOODBYE_TASK_STACK_SIZE,
		.state = TASK_STATE_WAITING,
        .params = NULL,
        .handle = NULL
    };
	*/

    mobile = (TaskModel){
    	.name = "MobileNet",
    	.taskFunc = mobilenetTask,
        .priority = 3,
        .type = TASK_TYPE_NPU,
		.deadlineType = DEADLINE_HARD,
        .releaseTime = 0,
        .period = 200000,
        .deadline = 200000,
        .capacity = 40000,
        .stackSize = MOBILENET_TASK_STACK_SIZE,
		.state = TASK_STATE_WAITING,
        .params = NULL,
        .handle = NULL
    };

    hello = (TaskModel){
    	.name = "Hello",
    	.taskFunc = helloTask,
        .priority = 3,
        .type = TASK_TYPE_CPU,
		.deadlineType = DEADLINE_HARD,
        .releaseTime = 0,
        .period = 100000,
        .deadline = 100000,
        .capacity = 40000,
        .stackSize = HELLO_TASK_STACK_SIZE,
		.state = TASK_STATE_WAITING,
        .params = NULL,
        .handle = NULL
    };

    /*
	taskD_EDF = (TaskModel){
		.name = "NPU_Soft",
		.taskFunc = taskFunctionEDF,
		.priority = 1,
		.type = TASK_TYPE_NPU,
		.deadlineType = DEADLINE_SOFT,
		.releaseTime = 50000,
		.period = 300000,
		.deadline = 300000,
		.capacity = 80000,
		.stackSize = DEMO_STACK_SIZE,
		.state = TASK_STATE_WAITING,
		.handle = NULL,
	};
	*/

    createCustomTask(&hello);
    createCustomTask(&mobile);
    //createCustomTask(&goodbye);
    //createCustomTask(&taskD_EDF);
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
        vTaskDelay(pdMS_TO_TICKS(task->capacity / 1000));
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
        vTaskDelay(pdMS_TO_TICKS(task->capacity / 1000));
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
