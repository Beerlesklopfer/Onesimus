/*
 * clockshift - shifts the wall clock of one process (LD_PRELOAD).
 *
 * The screenshot harness runs with the clock of the simulator: the simulator
 * starts its virtual time at a fixed moment (e.g. 2026-11-20 23:40) and lets
 * it run at speed 1. With this library the GUI sees the same time, so that
 * "7 h ago", today's date, the line of "now" in the timeline and the age of
 * a backup are the same in every run, whatever day it is made on.
 *
 * ONESIMUS_SCREENS_CLOCK_OFFSET: seconds to add to CLOCK_REALTIME (may be
 * negative). Only the wall clock is shifted; monotonic clocks (timers,
 * timeouts) are left alone.
 */
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

static long long offset_seconds(void)
{
    static int read = 0;
    static long long value = 0;
    if (!read) {
        const char *s = getenv("ONESIMUS_SCREENS_CLOCK_OFFSET");
        value = s ? strtoll(s, NULL, 10) : 0;
        read = 1;
    }
    return value;
}

int clock_gettime(clockid_t clk, struct timespec *ts)
{
    static int (*real)(clockid_t, struct timespec *) = NULL;
    if (!real)
        real = (int (*)(clockid_t, struct timespec *))dlsym(RTLD_NEXT, "clock_gettime");
    int r = real(clk, ts);
    if (r == 0 && ts && (clk == CLOCK_REALTIME || clk == CLOCK_REALTIME_COARSE))
        ts->tv_sec += offset_seconds();
    return r;
}

int gettimeofday(struct timeval *tv, void *tz)
{
    static int (*real)(struct timeval *, void *) = NULL;
    if (!real)
        real = (int (*)(struct timeval *, void *))dlsym(RTLD_NEXT, "gettimeofday");
    int r = real(tv, tz);
    if (r == 0 && tv)
        tv->tv_sec += offset_seconds();
    return r;
}

time_t time(time_t *t)
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    if (t)
        *t = ts.tv_sec;
    return ts.tv_sec;
}
