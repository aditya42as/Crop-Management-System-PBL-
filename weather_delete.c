// weather_delete.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "weather.h"

void delete_weather_for_user(const char *username) {
    load_weather_for_user(username);

    if (!weather_head) {
        printf("No weather records.\n");
        return;
    }

    char date[DATE_LEN];
    printf("Enter date (DD-MM-YYYY) to delete: ");
    if (fgets(date, sizeof(date), stdin) != NULL) {
        date[strcspn(date, "\n")] = '\0';
    } else {
        date[0] = '\0';
    }

    Weather *cur = weather_head;
    Weather *prev = NULL;
    while (cur) {
        if (strcmp(cur->date, date) == 0) {
            if (prev == NULL) {
                weather_head = cur->next;
            } else {
                prev->next = cur->next;
            }
            free(cur);
            save_weather_for_user(username);
            printf("Weather record deleted.\n");
            return;
        }
        prev = cur;
        cur = cur->next;
    }

    printf("Weather record not found.\n");
}
