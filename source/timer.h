/*
 * Copyright 2018-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * Enhanced timer.h - NPU-safe timing functions
 */

#ifndef _TIMER_H_
#define _TIMER_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @brief Initialize the timer system
 * Uses dedicated CTIMER instead of SysTick for NPU compatibility
 */
void TIMER_Init(void);

/*!
 * @brief Get current time in microseconds (32-bit)
 * @return Current time in microseconds
 * @note May overflow after ~71 minutes, use TIMER_GetTimeInUS64() for longer periods
 */
int TIMER_GetTimeInUS(void);

/*!
 * @brief Get current time in microseconds (64-bit)
 * @return Current time in microseconds
 * @note Provides much longer overflow period
 */
uint64_t TIMER_GetTimeInUS64(void);

/*!
 * @brief Get current CPU cycle count
 * @return Current CPU cycle count
 */
uint32_t TIMER_GetCycles(void);

/*!
 * @brief Convert CPU cycles to microseconds
 * @param cycles Number of CPU cycles
 * @return Time in microseconds
 */
uint32_t TIMER_CyclesToMicroseconds(uint32_t cycles);

/*!
 * @brief Safe duration calculation with overflow handling
 * @param startTime Start time in microseconds
 * @param endTime End time in microseconds
 * @return Duration in microseconds
 */
uint32_t TIMER_CalculateDuration(uint32_t startTime, uint32_t endTime);

/*!
 * @brief Get safe timestamp for NPU timing
 * @return Timestamp in microseconds, guaranteed not affected by NPU operations
 */
uint32_t TIMER_GetSafeTimestamp(void);

#ifdef __cplusplus
}
#endif

#endif /* _TIMER_H_ */
