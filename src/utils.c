/* utils.c
 * Timestamp formatting and helper utilities used across modules.
 */



#include <time.h>
#include <stdio.h>

double diff_seconds(struct timespec a, struct timespec b) {
    return (a.tv_sec - b.tv_sec) + (a.tv_nsec - b.tv_nsec) / 1e9;
}

void format_timestamp(char* buf, size_t size) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);

    struct tm tm_time;
    localtime_r(&ts.tv_sec, &tm_time);

    snprintf(buf, size,
        "%04d-%02d-%02d %02d:%02d:%02d.%03ld",
        tm_time.tm_year + 1900,
        tm_time.tm_mon + 1,
        tm_time.tm_mday,
        tm_time.tm_hour,
        tm_time.tm_min,
        tm_time.tm_sec,
        ts.tv_nsec / 1000000
    );
}
