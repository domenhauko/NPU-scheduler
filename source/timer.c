/*
 * Simplified NPU-safe timer.c - Using DWT cycle counter
 * This approach avoids complex CTIMER setup and uses the most reliable timing method
 */

#include "board.h"
#include "timer.h"
#include "fsl_clock.h"

#include <stdint.h>
#if defined(__ICCARM__) || defined(__ARMCC_VERSION) || defined(__REDLIB__)
#include <time.h>
#else
#include <sys/time.h>
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

static volatile uint32_t dwtStartTime = 0;
static volatile bool dwtInitialized = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

void TIMER_Init(void)
{
    // Initialize DWT (Data Watchpoint and Trace) cycle counter
    // This is the most accurate timing method and is not affected by NPU operations

    // Enable DWT
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

    // Reset and enable cycle counter
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    // Store the start time
    dwtStartTime = DWT->CYCCNT;
    dwtInitialized = true;
}

int TIMER_GetTimeInUS(void)
{
    if (!dwtInitialized) {
        TIMER_Init();
    }

    uint32_t currentCycles = DWT->CYCCNT;
    uint32_t elapsedCycles = currentCycles - dwtStartTime;

    // Convert cycles to microseconds
    // SystemCoreClock is the CPU frequency in Hz
    return (int)(elapsedCycles / (SystemCoreClock / 1000000));
}

uint64_t TIMER_GetTimeInUS64(void)
{
    if (!dwtInitialized) {
        TIMER_Init();
    }

    uint32_t currentCycles = DWT->CYCCNT;
    uint32_t elapsedCycles = currentCycles - dwtStartTime;

    // Convert cycles to microseconds (64-bit for longer periods)
    return ((uint64_t)elapsedCycles * 1000000) / SystemCoreClock;
}

uint32_t TIMER_GetCycles(void)
{
    if (!dwtInitialized) {
        TIMER_Init();
    }
    return DWT->CYCCNT;
}

uint32_t TIMER_CyclesToMicroseconds(uint32_t cycles)
{
    return cycles / (SystemCoreClock / 1000000);
}

uint32_t TIMER_GetSafeTimestamp(void)
{
    return TIMER_GetTimeInUS();
}

// Safe duration calculation that handles wraparound
uint32_t TIMER_CalculateDuration(uint32_t startTime, uint32_t endTime)
{
    if (endTime >= startTime) {
        return endTime - startTime;
    } else {
        // Handle wraparound case
        return (UINT32_MAX - startTime) + endTime + 1;
    }
}

// Legacy functions for compatibility
#if defined(__ARMCC_VERSION) || defined(__REDLIB__)

clock_t clock()
{
    return ((uint64_t)TIMER_GetTimeInUS() * CLOCKS_PER_SEC) / 1000000;
}

#elif defined(__ICCARM__)

int timespec_get(struct timespec* ts, int base)
{
    int us = TIMER_GetTimeInUS();
    ts->tv_sec = us / 1000000;
    ts->tv_nsec = (us % 1000000) * 1000;
    return TIME_UTC;
}

#else

int gettimeofday(struct timeval *__restrict __p, void *__restrict __tz)
{
    int us = TIMER_GetTimeInUS();
    __p->tv_sec = us / 1000000;
    __p->tv_usec = us % 1000000;
    return 0;
}

#endif
