/*
 * random_task_generator.h
 */

#ifndef RANDOM_TASK_GENERATOR_H
#define RANDOM_TASK_GENERATOR_H

#include "task_model.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Configuration for random task generation
typedef struct {
    uint32_t releaseTimeMin;    // Minimum release time (μs)
    uint32_t releaseTimeMax;    // Maximum release time (μs)
    uint32_t periodMin;         // Minimum period (μs)
    uint32_t periodMax;         // Maximum period (μs)
    uint32_t capacityMin;       // Minimum capacity (μs)
    uint32_t capacityMax;       // Maximum capacity (μs)
    uint32_t priorityMin;       // Minimum priority
    uint32_t priorityMax;       // Maximum priority
    uint32_t stackSizeMin;      // Minimum stack size
    uint32_t stackSizeMax;      // Maximum stack size
    float cpuTaskRatio;         // Ratio of CPU tasks (0.0-1.0)
} RandomTaskConfig;

// Function prototypes
void initRandomTaskGenerator(uint32_t seed);
TaskModel* generateRandomTask(const char* baseName, int taskId, const RandomTaskConfig* config);
void randomTask(void *pvParameters);
void createRandomTasks(int numTasks, const RandomTaskConfig* config);
void cleanupRandomTasks(void);
uint32_t getRandomNumber(uint32_t min, uint32_t max);

// Default configuration
extern const RandomTaskConfig DEFAULT_RANDOM_CONFIG;

#ifdef __cplusplus
}
#endif

#endif // RANDOM_TASK_GENERATOR_H
