// weather_manage.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "weather.h"
#include "weather_add.c"
#include "weather_update.c"
#include "weather_delete.c"
#include "weather_view.c"
#include "weather_predict.c"
#include "weather_io.c"

const char *get_current_user(); // from auth.c

Weather *weather_head = NULL;

void weather_manage_menu() {
    const char *user = get_current_user();
    if (!user || user[0] == '\0') {
        printf("No authenticated user. Please login first.\n");
        return;
    }

    int choice = 0;
    do {
        printf("\n=== WEATHER MENU ===\n");
        printf("1. Add Weather Data\n");
        printf("2. View Weather Data\n");
        printf("3. Update Weather Data\n");
        printf("4. Delete Weather Data\n");
        printf("5. Predict (stub)\n");
        printf("6. Go Back\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n') {
                
            }
            choice = 0;
        }
        getchar();

        switch (choice) {
            case 1: {
                add_weather_for_user(user);
            } break;

            case 2: {
                view_weather_for_user(user);
            } break;

            case 3: {
                update_weather_for_user(user);
            } break;

            case 4: {
                delete_weather_for_user(user);
            } break;

            case 5: {
                weather_predict_for_user(user);
            } break;

            case 6: {
                return;
            } break;

            default: {
                printf("Invalid choice.\n");
            } break;
        }
    } while (1);
}
