// historical_manage.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "historical.h"
#include "historical_basic.c"
#include "historical_advanced.c"
#include "historical_merge.c"

const char *get_current_user();

History *history_head = NULL;

void historical_manage() {
    const char *user = get_current_user();
    if (!user || user[0] == '\0') {
        printf("No authenticated user. Please login first.\n");
        return;
    }

    load_history_for_user(user);

    int choice = 0;
    do {
        printf("\n=== HISTORICAL DATA MENU ===\n");
        printf("1. Basic Estimate and Add\n");
        printf("2. Advanced Estimate and Add\n");
        printf("3. View Combined History\n");
        printf("4. Go Back\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n') {
                // consume
            }
            choice = 0;
        }
        getchar();

        switch (choice) {
            case 1: {
                estimate_basic_for_user(user);
            } break;

            case 2: {
                estimate_advanced_for_user(user);
            } break;

            case 3: {
                view_combined_history(user);
            } break;

            case 4: {
                save_history_for_user(user);
                return;
            } break;

            default: {
                printf("Invalid choice.\n");
            } break;
        }
    } while (1);
}
