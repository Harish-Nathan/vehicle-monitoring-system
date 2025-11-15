#ifndef DATA_H
#define DATA_H

#include <pthread.h>
#include <signal.h>

typedef struct {
    int speed;
    int fuel;
    int temp;

    int speed_alert;
    int fuel_alert;
    int temp_alert;

    struct timespec last_speed_alert_ts;
    struct timespec last_fuel_alert_ts;
    struct timespec last_temp_alert_ts;


    pthread_mutex_t lock;
    pthread_cond_t alert_cond;
} vehicle_data_t;

void format_timestamp(char* buf, size_t size);
void* ui_thread(void*);
double diff_seconds(struct timespec a, struct timespec b);

extern vehicle_data_t g_data;
extern volatile sig_atomic_t running;

void* speed_thread(void* arg);
void* fuel_thread(void* arg);
void* temp_thread(void* arg);
void* logger_thread(void* arg);

#endif
