/**
 * @file LinuxTimer.cpp
 *
 * @brief A linux OS timer that can be hooked to deliver timer messages. On each
 * tick of OS timer expiry, launch registered callback function.
 *
 **/


#include "LinuxTimer.h"
#include "log.h"
#include <unistd.h>
#include <signal.h>
#include <cstdlib>
#include <cstdio>
#include <cassert>

#define CLOCKID (CLOCK_REALTIME)
#define TIMERSIG (SIGRTMIN)

// converted to nano seconds
#define SYSTIMER_GRANULARITY_IN_NANOSECONDS (long long)(SYSTIMER_GRANULARITY_IN_MILLISECONDS *1000*1000)

/**
 * Signal handler for timer signal (TIMERSIG) */
static void osTimerHandler(int sig, siginfo_t *si, void *uc)
{
    // assert(sig == TIMERSIG && si->si_code == SI_TIMER);
#if (!defined NDEBUG)
    int ret = timer_getoverrun(*(timer_t*)si->si_value.sival_ptr);
    write(1, &ret, sizeof(int));
#endif   /* _DEBUG */

    // TODO: invoke default callback. Let it create as many timer messages and
    // post it to specific tasks

}

/* Check timer behavior */
int osTimerCreate(timer_t* const timerIdPtr)
{
    struct sigevent sev;
    struct sigaction sa;

    /* Setup timer handler for TIMERSIG. Users not allowed to change timer
     * handler. */
    printf("OS_TIMER: Setting up timer_handler for signal %d\n", TIMERSIG);
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = osTimerHandler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(TIMERSIG, &sa, NULL) == -1)
    {
        perror("OS_CALL: sigaction");
        return -1;
    }

    /* Create the timer */
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = TIMERSIG;
    sev.sigev_value.sival_ptr = timerIdPtr;
    if (timer_create(CLOCKID, &sev, timerIdPtr) == -1)
    {
        perror("OS_CALL: timer_create");
        return -1;
    }

    printf("OS_CALL: linux_timer_id: 0x%lx\n", (long) *timerIdPtr);
    return 0;
}

int osTimerStart(const timer_t timerId, const long long freq_nanosecs)
{
    struct itimerspec its;

    /* Start the timer */
    its.it_value.tv_sec  = freq_nanosecs / 1000000000;
    its.it_value.tv_nsec = freq_nanosecs % 1000000000;
    its.it_interval.tv_sec = its.it_value.tv_sec;
    its.it_interval.tv_nsec = its.it_value.tv_nsec;
    
    printf("OS_CALL: Timer frequency = %ld.%03ld s\n", 
           its.it_interval.tv_sec, its.it_interval.tv_nsec/(1000*1000));

    if (timer_settime(timerId, 0, &its, NULL) == -1)
    {
        perror("OS_CALL: timer_settime");
        return -1;
    }
    
    return 0;
}

int osTimerStop(const timer_t timerId)
{
    struct itimerspec its;

    /* Start the timer */
    its.it_value.tv_sec = its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = its.it_interval.tv_nsec = 0;

    if (timer_settime(timerId, 0, &its, NULL) == -1)
    {
        perror("OS_CALL: timer_settime");
        return -1;
    }
    return 0;
}


int osTimerDelete(timer_t timerId)
{
    if (timer_delete(timerId) == -1)
    {
        perror("OS_CALL: timer_delete()");
        return -1;
    }
    return 0;
}

void initLinuxTimer(timer_t* const timerIdPtr)
{
    int timerOpStatus = osTimerCreate(timerIdPtr);
    assert(0 == timerOpStatus);
    if (0 == timerOpStatus)
    {
        timerOpStatus = osTimerStart(*timerIdPtr, SYSTIMER_GRANULARITY_IN_NANOSECONDS);
        assert(0 == timerOpStatus);
    }
}

void finiLinuxTimer(timer_t timerId)
{
    int op = osTimerDelete(timerId);
    assert(op == 0);
}


/* eof */
