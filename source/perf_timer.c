/*
 * perf_timer.c
 *
 *  Created on: 27 Jun 2025
 *      Author: Domen
 */
#include "perf_timer.h"
#include "fsl_ctimer.h"
#include "fsl_clock.h"

// Define which CTIMER and clock source to use
#define PERF_TIMER           CTIMER0
#define PERF_TIMER_CLK_SRC   kCLOCK_Fro12M
#define PERF_TIMER_CLK_FREQ  CLOCK_GetFreq(PERF_TIMER_CLK_SRC)

void PERF_TIMER_Init(void)
{
    ctimer_config_t config;

    // 1. Attach the stable 12MHz FRO clock to the CTIMER
    CLOCK_AttachClk(PERF_TIMER_CLK_SRC);

    // 2. Get the default CTIMER configuration
    CTIMER_GetDefaultConfig(&config);

    // 3. Set the prescaler to 0 so the timer runs at full speed (12 MHz)
    config.prescale = 0;

    // 4. Initialize the CTIMER and start it
    CTIMER_Init(PERF_TIMER, &config);
    CTIMER_StartTimer(PERF_TIMER);
}

uint32_t PERF_TIMER_GetTimeInUS(void)
{
    // The CTIMER's clock frequency is 12,000,000 Hz.
    // To convert the raw count to microseconds, we can use:
    // us = (count * 1,000,000) / frequency
    // To avoid floating point math, we can rearrange this:
    // us = (count * 10) / (frequency / 100,000)
    // us = (count * 10) / 120 -> simplified to avoid large intermediate values

    uint32_t current_count = (uint32_t)CTIMER_GetTimerCountValue(PERF_TIMER);

    // Perform calculation with 64-bit integers to prevent overflow
    return (current_count * 1000000) / PERF_TIMER_CLK_FREQ;
}

