// weather_view.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "weather.h"



void view_weather_for_user(const char *username) {
    load_weather_for_user(username);

    if (!weather_head) {
        printf("\nNo weather records available.\n");
        return;
    }

    printf("\n========================= WEATHER RECORDS =========================\n");
    printf("%-5s %-15s %-18s %-18s %-15s\n",
           "No", "Date", "Temperature (C)", "Humidity (%)", "Rainfall (mm)");
    printf("-------------------------------------------------------------------\n");

    Weather *t = weather_head;
    int i = 1;
    while (t) {
        printf("%-5d %-15s %-18.2f %-18.2f %-15.2f\n",
               i++, t->date, t->temperature, t->humidity, t->rainfall);
        t = t->next;
    }

    printf("-------------------------------------------------------------------\n");
}
