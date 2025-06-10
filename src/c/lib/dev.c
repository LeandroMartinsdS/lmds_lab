#include "dev.h"
#include <time.h>
#include <sched.h>

void sleep_until(struct timespec* time, long interval_ns) {
    time->tv_nsec += interval_ns;
    while (time->tv_nsec >= 1000000000L) {
        time->tv_nsec -= 1000000000L;
        time->tv_sec += 1;
    }
    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, time, NULL);
}

void timed_loop(long interval_ns) {
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    while (1) {
        sleep_until(&time, interval_ns);
        // Perform periodic task here
        printf("Periodic task executed.\n");
    }
}


void dev_init() {
    // Initialize device-specific settings or configurations here
    printf("Device initialized.\n");
}

void main(void) {
    // Call the device initialization function
    dev_init();
    // Additional main function logic can be added here
    printf("Main function executed.\n");

    return;
}