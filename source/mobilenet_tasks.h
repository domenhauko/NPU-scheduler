/*
 * mobilenet_tasks.h
 *
 *  Created on: 24 Jul 2025
 *      Author: Domen
 */

#ifndef MOBILENET_TASKS_H_
#define MOBILENET_TASKS_H_

#define MOBILENET_TASK_STACK_SIZE 4096
#define HELLO_TASK_STACK_SIZE     256
#define GOODBYE_TASK_STACK_SIZE   256
#define DEMO_STACK_SIZE configMINIMAL_STACK_SIZE + 100

#include "task_model.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


// Function prototypes
void createMobileNetTasks(void);
void mobilenetTask(void *pvParameters);
void helloTask(void *pvParameters);
void goodbyeTask(void *pvParameters);
void taskFunctionEDF(void *pvParameters);

#ifdef __cplusplus
}
#endif


#endif /* MOBILENET_TASKS_H_ */
