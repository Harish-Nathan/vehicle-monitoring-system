/* logger.c
 * Thread-safe JSON logging utilities; write timestamped events to logs/vehicle.log
 */



#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <time.h>
#include "data.h"

// Buffer size for timestamp string
#define TSBUF 64

void* logger_thread(void* arg) {

    FILE* fp = fopen("logs/vehicle.log", "a");
    if (!fp) {
        perror("log file");
        return NULL;
    }

    while (running) {

        // 1-second timeout
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += 1;

        pthread_mutex_lock(&g_data.lock);

        // Wait for alert OR timeout
        pthread_cond_timedwait(&g_data.alert_cond, &g_data.lock, &ts);

        // Copy values
        int s  = g_data.speed;
        int f  = g_data.fuel;
        int t  = g_data.temp;

        int sa = g_data.speed_alert;
        int fa = g_data.fuel_alert;
        int ta = g_data.temp_alert;

        // Hysteresis-based clearing
        if (sa && s < 115) g_data.speed_alert = 0;
        if (fa && f > 18)  g_data.fuel_alert  = 0;
        if (ta && t < 95)  g_data.temp_alert  = 0;

        pthread_mutex_unlock(&g_data.lock);

        // -------- TIMESTAMP --------
        char tsbuf[TSBUF];
        format_timestamp(tsbuf, sizeof(tsbuf));

        // -------- ALERT STRING -----
        const char* alert_str = "none";
        if (sa)      alert_str = "speed_high";
        else if (fa) alert_str = "fuel_low";
        else if (ta) alert_str = "temp_high";

        // -------- JSON FORMAT OUTPUT --------
        printf(
            "{\"ts\":\"%s\", \"speed\":%d, \"fuel\":%d, \"temp\":%d, \"alert\":\"%s\"}\n",
            tsbuf, s, f, t, alert_str
        );

        fprintf(
            fp,
            "{\"ts\":\"%s\", \"speed\":%d, \"fuel\":%d, \"temp\":%d, \"alert\":\"%s\"}\n",
            tsbuf, s, f, t, alert_str
        );

        fflush(fp);
    }

    fclose(fp);
    return NULL;
}
