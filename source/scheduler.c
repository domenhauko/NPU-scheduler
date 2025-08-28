/*
 * Advanced scheduler.c implementing priority-based scheduling with EDF tie-breaking
 * and smart preemption logic for hard/soft deadline tasks
 *
 *
 *
 * custom_TaskModel_preempt
 */

#include "scheduler.h"
#include "fsl_debug_console.h"
#include "timer.h"
#include "FreeRTOS.h"
#include "task.h"
#include "task_model.h"
#include "task_utils.h"

#include <stdint.h>
#include <stdbool.h>

#define CONTEXT_SWITCH 0
#define PREEMPT_CAPACITY 0
#define BUFFER 30000

static volatile int cpuBusy = 0;
static volatile int npuBusy = 0;

// Use DWT-based timing for NPU safety
uint32_t schedulerStartTime = 0;

// Task execution tracking with cycle-accurate timing
static uint32_t cpuTaskStartTime = 0;
static uint32_t npuTaskStartTime = 0;
static TaskModel* currentCpuTask = NULL;
static TaskModel* currentNpuTask = NULL;

// Statistics tracking
static uint32_t totalCpuExecutions = 0;
static uint32_t totalNpuExecutions = 0;
static uint32_t missedHardDeadlines = 0;
static uint32_t missedSoftDeadlines = 0;
static uint32_t totalPreemptions = 0;

// Forward declarations
static TaskModel* selectNextTask(TaskType resourceType, uint32_t currentTime);
static bool shouldPreemptTask(TaskModel* currentTask, TaskModel* newTask, uint32_t currentTime);
static void preemptTask(TaskModel* taskToPreempt);
static void updateTaskStates(uint32_t currentTime);
static int compareTasksPriority(TaskModel* task1, TaskModel* task2);

void setCpuBusy(int busy) {
    uint32_t now = calculateDuration(schedulerStartTime, TIMER_GetTimeInUS());

    if (busy && !cpuBusy) {
        // Task starting
        cpuBusy = 1;
        cpuTaskStartTime = TIMER_GetTimeInUS();
        if (currentCpuTask) {
            currentCpuTask->state = TASK_STATE_RUNNING;
            addTaskStartTime(currentCpuTask, now);
            PRINTF("[SCHEDULER] CPU task %s STARTED at %u us (Priority: %u, Deadline: %u)\r\n",
                   currentCpuTask->name, now, currentCpuTask->priority, currentCpuTask->absoluteDeadline);
        }
    } else if (!busy && cpuBusy) {
        // Task completing
        uint32_t endTime = TIMER_GetTimeInUS();
        uint32_t duration = calculateDuration(cpuTaskStartTime, endTime);

        if (currentCpuTask) {
            addTaskEndTime(currentCpuTask, now);
            PRINTF("[SCHEDULER] CPU task %s COMPLETED at %u us (Duration: %u us)\r\n",
                   currentCpuTask->name, now, duration);

            // Reset task state and update next release time for periodic tasks
            currentCpuTask->state = TASK_STATE_WAITING;
            //currentCpuTask->releaseTime = now + currentCpuTask->period;				/// TOLE NI OKEJ!!! PERIODA SE NE ZAČNE ŠELE PO KONČANJU
            //currentCpuTask->absoluteDeadline = currentCpuTask->releaseTime + currentCpuTask->deadline;
        }

        cpuBusy = 0;
        currentCpuTask = NULL;
        totalCpuExecutions++;
    }
}

void setNpuBusy(int busy) {
    uint32_t now = calculateDuration(schedulerStartTime, TIMER_GetTimeInUS());

    if (busy && !npuBusy) {
        // Task starting
        npuBusy = 1;
        npuTaskStartTime = TIMER_GetTimeInUS();
        if (currentNpuTask) {
            currentNpuTask->state = TASK_STATE_RUNNING;
            addTaskStartTime(currentNpuTask, now);
            PRINTF("[SCHEDULER] NPU task %s STARTED at %u us (Priority: %u, Deadline: %u)\r\n",
                   currentNpuTask->name, now, currentNpuTask->priority, currentNpuTask->absoluteDeadline);
        }
    } else if (!busy && npuBusy) {
        // Task completing
        uint32_t endTime = TIMER_GetTimeInUS();
        uint32_t duration = calculateDuration(npuTaskStartTime, endTime);

        if (currentNpuTask) {
            addTaskEndTime(currentNpuTask, now);
            PRINTF("[SCHEDULER] NPU task %s COMPLETED at %u us (Duration: %u us)\r\n",
                   currentNpuTask->name, now, duration);

            // Reset task state and update next release time for periodic tasks
            currentNpuTask->state = TASK_STATE_WAITING;
            //currentNpuTask->releaseTime = now + currentNpuTask->period;
            //currentNpuTask->absoluteDeadline = currentNpuTask->releaseTime + currentNpuTask->deadline;
        }

        npuBusy = 0;
        currentNpuTask = NULL;
        totalNpuExecutions++;
    }
}

static void updateTaskStates(uint32_t currentTime) {
    for (int i = 0; i < getTaskCount(); i++) {
        TaskModel *task = getTaskListElement(i);

        // Check for missed deadlines
        if (task->state != TASK_STATE_WAITING && currentTime > task->absoluteDeadline) {
            if (task->deadlineType == DEADLINE_HARD) {
                missedHardDeadlines++;
                PRINTF("[CRITICAL ERROR] Hard deadline missed by task %s! Time: %u, Deadline: %u\r\n",
                       task->name, currentTime, task->absoluteDeadline);
                PRINTF("[SYSTEM HALT] Hard deadline violation - stopping system!\r\n");
                printAllTaskDurations();
                while(1); // Halt the system
                task->state = TASK_STATE_WAITING; // For now continue and log for timing
            } else {
                if (task->state != TASK_STATE_PREEMPTED) { // Only log once for soft misses
                    missedSoftDeadlines++;
                    PRINTF("[WARNING] Soft deadline missed by task %s! Time: %u, Deadline: %u (Diff: %u us)\r\n",
                           task->name, currentTime, task->absoluteDeadline,
                           currentTime - task->absoluteDeadline);
                    // Mark as waiting to prevent repeated logs for the same missed deadline
                    task->state = TASK_STATE_WAITING;
                }
            }
        }

        // Update task states based on release times
        if (currentTime >= task->releaseTime && task->state == TASK_STATE_WAITING) {
            uint32_t jobRelease = task->releaseTime;
        	task->state = TASK_STATE_READY;
            task->absoluteDeadline = jobRelease + task->deadline;

            PRINTF("[SCHEDULER] Task %s is now READY (Release: %u, Deadline: %u)\r\n",
                   task->name, task->releaseTime, task->absoluteDeadline);
            // Schedule the *next* release
            task->releaseTime = jobRelease + task->period;
        }
    }
}

static int compareTasksPriority(TaskModel* task1, TaskModel* task2) {
    // Higher priority number = higher priority
    if (task1->priority != task2->priority) {
        return (task1->priority > task2->priority) ? -1 : 1;
    }

    // EDF tie-breaking: earlier deadline = higher priority
    if (task1->absoluteDeadline != task2->absoluteDeadline) {
        return (task1->absoluteDeadline < task2->absoluteDeadline) ? -1 : 1;
    }

    return 0; // Equal priority and deadline
}

static TaskModel* selectNextTask(TaskType resourceType, uint32_t currentTime) {
    TaskModel* bestTask = NULL;

    for (int i = 0; i < getTaskCount(); i++) {
        TaskModel *task = getTaskListElement(i);

        // Skip tasks that don't match resource type or aren't ready
        if (task->type != resourceType || task->state != TASK_STATE_READY) {
            continue;
        }

        // Find the highest priority task (with EDF tie-breaking)
        if (bestTask == NULL || compareTasksPriority(task, bestTask) < 0) {
            bestTask = task;
        }
    }

    return bestTask;
}

static bool shouldPreemptTask(TaskModel* currentTask, TaskModel* newTask, uint32_t currentTime) {

	PRINTF("Should preempt?\r\n");
    if (currentTask == NULL || newTask == NULL) {
        return false;
    }

    // Calculate time until new task's deadline
    uint32_t timeUntilNewDeadline = (newTask->absoluteDeadline > currentTime) ?
                                    (newTask->absoluteDeadline - currentTime) : 0;

    // Preemption rules based on our algorithm
    if (currentTask->deadlineType == DEADLINE_HARD && newTask->deadlineType == DEADLINE_HARD) {
        // Hard → Hard: preempt only if higher priority and in "danger zone"
        return (newTask->priority > currentTask->priority) &&
        	   (timeUntilNewDeadline <= (newTask->capacity + PREEMPT_CAPACITY + CONTEXT_SWITCH + BUFFER));
    }

    if (currentTask->deadlineType == DEADLINE_SOFT && newTask->deadlineType == DEADLINE_HARD) {
        // Hard → Soft: preempt if hard task is in danger zone
        return (timeUntilNewDeadline <= (newTask->capacity + PREEMPT_CAPACITY + CONTEXT_SWITCH + BUFFER));
    }

    // Soft tasks are non-preemptive among themselves
    // Hard tasks don't preempt soft tasks unless in danger zone (covered above)
    return false;
}

static void preemptTask(TaskModel* taskToPreempt) {
    if (taskToPreempt == NULL) {
        return;
    }

    PRINTF("[SCHEDULER] Preempting task %s\r\n", taskToPreempt->name);

    // Mark task as preempted (will restart when scheduled again)
    taskToPreempt->state = TASK_STATE_PREEMPTED;
    totalPreemptions++;


    // vTaskSuspend suspends the preempted task and the task is recreated later
    if (taskToPreempt->handle != NULL) {
        vTaskSuspend(taskToPreempt->handle);
        taskToPreempt->handle = NULL;
    }


    // Recreate the task (it will restart from the beginning)
    BaseType_t result = xTaskCreate(
        taskToPreempt->taskFunc,
        taskToPreempt->name,
        taskToPreempt->stackSize,
        (void*)taskToPreempt,
        taskToPreempt->priority, // Use the original FreeRTOS priority
        &(taskToPreempt->handle)
    );

    if (result != pdPASS) {
        PRINTF("[ERROR] Failed to recreate preempted task %s\r\n", taskToPreempt->name);
    }

}

void printSchedulerStats() {
    static uint32_t lastStatsTime = 0;
    static uint32_t statsCounter = 0;
    uint32_t now = calculateDuration(schedulerStartTime, TIMER_GetTimeInUS());

    // Print stats every 2 seconds and only when no tasks are busy
    if (now - lastStatsTime > 1000000 && cpuBusy == 0 && npuBusy == 0) {
        statsCounter++;
        PRINTF("\r\n=== ADVANCED SCHEDULER STATS #%u ===\r\n", statsCounter);
        PRINTF("Uptime: %u seconds\r\n", now / 1000000);
        PRINTF("CPU Executions: %u\r\n", totalCpuExecutions);
        PRINTF("NPU Executions: %u\r\n", totalNpuExecutions);
        PRINTF("Hard Deadline Misses: %u\r\n", missedHardDeadlines);
        PRINTF("Soft Deadline Misses: %u\r\n", missedSoftDeadlines);
        PRINTF("Total Preemptions: %u\r\n", totalPreemptions);
        PRINTF("CPU Clock: %u MHz\r\n", SystemCoreClock / 1000000);
        PRINTF("Resources: CPU=%s, NPU=%s\r\n",
               cpuBusy ? "BUSY" : "FREE",
               npuBusy ? "BUSY" : "FREE");
        PRINTF("=====================================\r\n\r\n");
        lastStatsTime = now;

        printAllTaskDurations();
    }
}

void runTaskScheduler(void* params) {
    TIMER_Init(); // Initialize DWT-based timing
    schedulerStartTime = TIMER_GetTimeInUS();
    PRINTF("[SCHEDULER] Advanced scheduler started at %u us\r\n", schedulerStartTime);

    // Initialize all tasks to waiting state
    for (int i = 0; i < getTaskCount(); i++) {
        TaskModel *task = getTaskListElement(i);
        task->state = TASK_STATE_WAITING;
        task->absoluteDeadline = task->releaseTime + task->deadline;
    }

    while (1) {
        uint32_t now = calculateDuration(schedulerStartTime, TIMER_GetTimeInUS());

        // Update all task states
        updateTaskStates(now);

        // Handle CPU resource scheduling
        if (!cpuBusy) {
            // No CPU task running, select next task
            TaskModel* nextCpuTask = selectNextTask(TASK_TYPE_CPU, now);
            if (nextCpuTask != NULL) {
                currentCpuTask = nextCpuTask;
                setCpuBusy(true);

                if (nextCpuTask->handle != NULL) {
                    xTaskNotifyGive(nextCpuTask->handle);
                }
            }
        } else {
            // CPU task is running, check for preemption
            TaskModel* nextCpuTask = selectNextTask(TASK_TYPE_CPU, now);
            if (nextCpuTask != NULL && nextCpuTask != currentCpuTask) {
                if (shouldPreemptTask(currentCpuTask, nextCpuTask, now)) {
                    TaskModel* taskToPreempt = currentCpuTask;
                    uint32_t beforePreemption = TIMER_GetTimeInUS();
                    PRINTF("[SCHEDULER]: Time before preemption: %d\r\n", beforePreemption);
                    preemptTask(taskToPreempt);
                    uint32_t afterPreemption = TIMER_GetTimeInUS();
                    PRINTF("[SCHEDULER]: Time after preemption: %d\r\n", afterPreemption);
                    PRINTF("[SCHEDULER]: Time to preempt: %d\r\n", calculateDuration(beforePreemption, afterPreemption));

                    // The CPU is still busy, just with the new task.
                    // Manually update state for the new task.
                    currentCpuTask = nextCpuTask;
                    cpuTaskStartTime = TIMER_GetTimeInUS();
                    currentCpuTask->state = TASK_STATE_RUNNING;

                    uint32_t now_after_preempt = calculateDuration(schedulerStartTime, TIMER_GetTimeInUS());
                    addTaskStartTime(currentCpuTask, now_after_preempt);
                    PRINTF("[SCHEDULER] CPU task %s STARTED after preemption at %u us (Priority: %u, Deadline: %u)\r\n",
                           currentCpuTask->name, now_after_preempt, currentCpuTask->priority, currentCpuTask->absoluteDeadline);

                    if (currentCpuTask->handle != NULL) {
                    	xTaskNotifyGive(currentCpuTask->handle);
                    }
                }
            }
        }

        // Handle NPU resource scheduling (same logic as CPU)
        if (!npuBusy) {
            TaskModel* nextNpuTask = selectNextTask(TASK_TYPE_NPU, now);
            if (nextNpuTask != NULL) {
                currentNpuTask = nextNpuTask;
                setNpuBusy(true);

                if (nextNpuTask->handle != NULL) {
                    xTaskNotifyGive(nextNpuTask->handle);
                }
            }
        } else {
            TaskModel* nextNpuTask = selectNextTask(TASK_TYPE_NPU, now);
            if (nextNpuTask != NULL && nextNpuTask != currentNpuTask) {
                if (shouldPreemptTask(currentNpuTask, nextNpuTask, now)) {
                    TaskModel* taskToPreempt = currentNpuTask;
                    preemptTask(taskToPreempt);

                    currentNpuTask = nextNpuTask;
                    npuTaskStartTime = TIMER_GetTimeInUS();
                    currentNpuTask->state = TASK_STATE_RUNNING;

                    uint32_t now_after_preempt = calculateDuration(schedulerStartTime, TIMER_GetTimeInUS());
                    addTaskStartTime(currentNpuTask, now_after_preempt);
                    PRINTF("[SCHEDULER] NPU task %s STARTED after preemption at %u us (Priority: %u, Deadline: %u)\r\n",
                           currentNpuTask->name, now_after_preempt, currentNpuTask->priority, currentNpuTask->absoluteDeadline);

                    if (currentNpuTask->handle != NULL) {
                        xTaskNotifyGive(currentNpuTask->handle);
                    }
                }
            }
        }

        // Print periodic statistics (only when idle)
        if (cpuBusy == 0 && npuBusy == 0) {
            printSchedulerStats();
        }

        // Check scheduler periodically. A small delay is crucial to prevent starving other tasks.
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}
