/*
 * exampleOne_tasks.h
 *
 *  Created on: 24 Jul 2025
 *      Author: Domen
 */

#ifndef EXAMPLEONE_TASKS_H_
#define EXAMPLEONE_TASKS_H_

#define TASK1_TASK_STACK_SIZE	256
#define TASK2_TASK_STACK_SIZE 	4096
#define TASK3_TASK_STACK_SIZE   256
#define TASK4_TASK_STACK_SIZE   512
#define TASK5_TASK_STACK_SIZE   256

#define TASK1_TASK_PRIORITY     2
#define TASK2_TASK_PRIORITY   	2
#define TASK3_TASK_PRIORITY     2
#define TASK4_TASK_PRIORITY     1
#define TASK5_TASK_PRIORITY     4

#include "task_model.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


// Function prototypes
void createExampleOneTasks(void);
void taskFunction(void *pvParameters);

#ifdef __cplusplus
}
#endif

#endif /* EXAMPLEONE_TASKS_H_ */
