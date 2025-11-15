/* fuel.c
 * Simulate fuel-level sensor, threshold checking, and event generation.
 */



#include "data.h"
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>

void* fuel_thread(void* arg) {

    while (running) {
        pthread_mutex_lock(&g_data.lock);

        // simulate fuel drop, never below 0
        if (g_data.fuel > 0)
            g_data.fuel -= 1;

        // Hysteresis thresholds
        int trigger = (g_data.fuel < 15);
        int clear_allowed = (g_data.fuel > 18);

        struct timespec now;
        clock_gettime(CLOCK_REALTIME, &now);

        double elapsed = diff_seconds(now, g_data.last_fuel_alert_ts);

        if (trigger && elapsed >= 5.0 && g_data.fuel_alert == 0) {
            g_data.fuel_alert = 1;
            g_data.last_fuel_alert_ts = now;
            pthread_cond_signal(&g_data.alert_cond);
        }

        pthread_mutex_unlock(&g_data.lock);
        sleep(1);   // 1 second
    }

    return NULL;
}
