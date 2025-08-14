// scheduler.h
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "task_model.h"
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

void runTaskScheduler(void* params);
void setCpuBusy(int busy);
void setNpuBusy(int busy);
void printSchedulerStats(void);


extern uint32_t schedulerStartTime;

#ifdef __cplusplus
}
#endif

#endif // SCHEDULER_H
