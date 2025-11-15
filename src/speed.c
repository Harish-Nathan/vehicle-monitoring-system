/* speed.c
 * Simulate speed sensor, apply hysteresis and debounce, update shared state.
 */


#include "data.h"
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>

void* speed_thread(void* arg) {
    srand(time(NULL) + 1); // seed

    while (running) {
        int new_speed = rand() % 181;   // 0–180

        pthread_mutex_lock(&g_data.lock);
        g_data.speed = new_speed;

        // Hysteresis thresholds
        int trigger = (g_data.speed > 120);
        int clear_allowed = (g_data.speed < 115);

        // Current time for debounce logic
        struct timespec now;
        clock_gettime(CLOCK_REALTIME, &now);

        // Time since last alert
        double elapsed = diff_seconds(now, g_data.last_speed_alert_ts);

        // Trigger alert if:
        // - threshold exceeded
        // - at least 5 sec since last alert
        // - alert is not already active
        if (trigger && elapsed >= 5.0 && g_data.speed_alert == 0) {
            g_data.speed_alert = 1;
            g_data.last_speed_alert_ts = now;
            pthread_cond_signal(&g_data.alert_cond);
        }

        pthread_mutex_unlock(&g_data.lock);
        usleep(200000);  // 200ms
    }

    return NULL;
}
