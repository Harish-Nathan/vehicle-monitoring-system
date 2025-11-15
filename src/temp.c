/* temp.c
 * Simulate engine temperature sensor and trigger temperature alerts.
 */



#include "data.h"
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>

void* temp_thread(void* arg) {
    srand(time(NULL) + 2); // different seed

    while (running) {
        pthread_mutex_lock(&g_data.lock);

        // random walk delta: -2 to +2
        int delta = (rand() % 5) - 2;
        g_data.temp += delta;

        // clamp range 60–120
        if (g_data.temp < 60) g_data.temp = 60;
        if (g_data.temp > 120) g_data.temp = 120;

        // Hysteresis
        int trigger = (g_data.temp > 100);
        int clear_allowed = (g_data.temp < 95);

        struct timespec now;
        clock_gettime(CLOCK_REALTIME, &now);

        double elapsed = diff_seconds(now, g_data.last_temp_alert_ts);

        if (trigger && elapsed >= 5.0 && g_data.temp_alert == 0) {
            g_data.temp_alert = 1;
            g_data.last_temp_alert_ts = now;
            pthread_cond_signal(&g_data.alert_cond);
        }

        pthread_mutex_unlock(&g_data.lock);

        usleep(500000); // 500 ms
    }

    return NULL;
}
