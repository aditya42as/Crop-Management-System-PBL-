// weather_update.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "weather.h"

void update_weather_for_user(const char *username) {
    load_weather_for_user(username);

    if (!weather_head) {
        printf("No weather records.\n");
        return;
    }

    char date[DATE_LEN];
    printf("Enter date (DD-MM-YYYY) to update: ");
    if (fgets(date, sizeof(date), stdin) != NULL) {
        date[strcspn(date, "\n")] = '\0';
    } else {
        date[0] = '\0';
    }

    Weather *cur = weather_head;
    while (cur) {
        if (strcmp(cur->date, date) == 0) {
            printf("Editing record for %s\n", date);

            printf("New Temperature (current %.2f): ", cur->temperature);
            float f;
            if (scanf("%f", &f) == 1) {
                cur->temperature = f;
            }
            getchar();

            printf("New Humidity (current %.2f): ", cur->humidity);
            if (scanf("%f", &f) == 1) {
                cur->humidity = f;
            }
            getchar();

            printf("New Rainfall (current %.2f): ", cur->rainfall);
            if (scanf("%f", &f) == 1) {
                cur->rainfall = f;
            }
            getchar();

            save_weather_for_user(username);
            printf("Weather record updated.\n");
            return;
        }
        cur = cur->next;
    }

    printf("Weather record not found.\n");
}
