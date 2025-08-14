/*
 * perf_timer.h
 *
 *  Created on: 27 Jun 2025
 *      Author: Domen
 */

#ifndef PERF_TIMER_H
#define PERF_TIMER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes the performance timer.
 *
 * Configures a CTIMER to run continuously on a stable clock source,
 * providing a reliable time base for measurements.
 */
void PERF_TIMER_Init(void);

/**
 * @brief Gets the current time in microseconds.
 *
 * @return The current time in microseconds from the performance timer.
 */
uint32_t PERF_TIMER_GetTimeInUS(void);

#ifdef __cplusplus
}
#endif

#endif /* PERF_TIMER_H */
