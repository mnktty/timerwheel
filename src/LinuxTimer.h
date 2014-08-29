#ifndef _LINUXTIMER_H_
#define _LINUXTIMER_H_

/**
 * @file LinuxTimer.h
 *
 * @brief Interface for linux timer */

#include <ctime>

#if (defined __cplusplus)
extern "C" {
#endif  /* __cplusplus */

void initLinuxTimer(timer_t* const timerIdPtr);
void finiLinuxTimer(timer_t timerId);

/**
 * Default frequency of 500 ms for system timer. Feel free to adjust this to
 * something else. */
const int SYSTIMER_GRANULARITY_IN_MILLISECONDS = 500;

#if (defined __cplusplus)
}
#endif  /* __cplusplus */

#endif /* _LINUXTIMER_H_ */
