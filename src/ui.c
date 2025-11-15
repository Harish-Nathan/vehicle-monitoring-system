/* ui.c
 * ncurses-based dashboard rendering current sensor readings and alert status.
 */


#include <ncurses.h>
#include <unistd.h>
#include "data.h"

void* ui_thread(void* arg) {
    initscr(); // start ncurses mode
    noecho(); // don’t show typed keys
    curs_set(FALSE); // hide the blinking cursor

    while (running) {
        pthread_mutex_lock(&g_data.lock);
        int s = g_data.speed;
        int f = g_data.fuel;
        int t = g_data.temp;
        int sa = g_data.speed_alert;
        int fa = g_data.fuel_alert;
        int ta = g_data.temp_alert;
        pthread_mutex_unlock(&g_data.lock);

        clear();
        mvprintw(1, 2, "Real-Time Vehicle Monitor");
        mvprintw(3, 2, "Speed : %d km/h", s);
        mvprintw(4, 2, "Fuel  : %d %%", f);
        mvprintw(5, 2, "Temp  : %d C", t);

        if (sa)      mvprintw(7, 2, "ALERT: SPEED HIGH");
        else if (fa) mvprintw(7, 2, "ALERT: FUEL LOW");
        else if (ta) mvprintw(7, 2, "ALERT: TEMP HIGH");
        else         mvprintw(7, 2, "ALERT: NONE");

        refresh();
        usleep(200000);
    }

    endwin();
    return NULL;
}
