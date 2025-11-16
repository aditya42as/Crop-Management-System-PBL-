// weather_add.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "weather.h"

void add_weather_for_user(const char *username) {
    load_weather_for_user(username);

    Weather *w = (Weather *)malloc(sizeof(Weather));
    if (!w) {
        printf("Memory allocation failed.\n");
        return;
    }

    printf("\n=== ADD WEATHER DATA ===\n");
    printf("Date (DD-MM-YYYY): ");
    if (fgets(w->date, sizeof(w->date), stdin) != NULL) {
        w->date[strcspn(w->date, "\n")] = '\0';
    } else {
        w->date[0] = '\0';
    }

    printf("Temperature (°C): ");
    if (scanf("%f", &w->temperature) != 1) {
        w->temperature = 0.0f;
    }
    getchar();

    printf("Humidity (%%): ");
    if (scanf("%f", &w->humidity) != 1) {
        w->humidity = 0.0f;
    }
    getchar();

    printf("Rainfall (mm): ");
    if (scanf("%f", &w->rainfall) != 1) {
        w->rainfall = 0.0f;
    }
    getchar();

    w->next = weather_head;
    weather_head = w;

    save_weather_for_user(username);
    printf("Weather record added!\n");
}
