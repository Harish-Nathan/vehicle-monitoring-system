/* main.c
 * Initialize data, spawn sensor/logger/ui threads, handle SIGINT,
 * coordinate graceful shutdown and thread joins.
 */


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include "data.h"

vehicle_data_t g_data;
volatile sig_atomic_t running = 1;

// ----------- SIGINT HANDLER -------------
void handle_sigint(int sig) {
    running = 0;

    // Wake logger (if waiting)
    pthread_cond_broadcast(&g_data.alert_cond);

    // Optional: print shutdown msg
    write(STDOUT_FILENO, "\nSIGINT received, shutting down...\n", 35);
}
// -----------------------------------------

int main() {
    // Attach signal handler
    signal(SIGINT, handle_sigint);

    // Init shared data
    g_data.speed = 0;
    g_data.fuel = 100;
    g_data.temp = 60;

    g_data.speed_alert = 0;
    g_data.fuel_alert  = 0;
    g_data.temp_alert  = 0;

    // Init timestamps
    clock_gettime(CLOCK_REALTIME, &g_data.last_speed_alert_ts);
    clock_gettime(CLOCK_REALTIME, &g_data.last_fuel_alert_ts);
    clock_gettime(CLOCK_REALTIME, &g_data.last_temp_alert_ts);

    // Init mutex + condition variable
    pthread_mutex_init(&g_data.lock, NULL);
    pthread_cond_init(&g_data.alert_cond, NULL);

    // Create threads
    pthread_t t_speed, t_fuel, t_temp, t_logger;

    pthread_create(&t_speed, NULL, speed_thread, NULL);
    pthread_create(&t_fuel, NULL, fuel_thread, NULL);
    pthread_create(&t_temp,   NULL, temp_thread,  NULL);
    pthread_create(&t_logger, NULL, logger_thread, NULL);

    pthread_t t_ui;
    pthread_create(&t_ui, NULL, ui_thread, NULL);

    // Main thread waits until running==0
    while (running) {
        sleep(1);
    }

    // Join threads cleanly
    pthread_join(t_speed, NULL);
    pthread_join(t_fuel,  NULL);
    pthread_join(t_temp,  NULL);
    pthread_join(t_logger,NULL);

    pthread_join(t_ui, NULL);
    // Destroy sync primitives
    pthread_mutex_destroy(&g_data.lock);
    pthread_cond_destroy(&g_data.alert_cond);

    printf("Shutdown complete.\n");
    return 0;
}
