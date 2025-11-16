// crop_manage.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crop.h"
#include "crop_io.c"
#include "add_crop.c"
#include "delete_crop.c"
#include "update_crop.c"
#include "view_crop.c"


// get_current_user declared in auth.c
const char *get_current_user();

Crop *crop_head = NULL;

void crop_manage_menu() {
    const char *user = get_current_user();
    if (!user || user[0] == '\0') {
        printf("No authenticated user. Please login first.\n");
        return;
    }

    int choice = 0;
    do {
        printf("\n=== CROP MANAGEMENT ===\n");
        printf("1. Add Crop\n");
        printf("2. Update Crop\n");
        printf("3. Delete Crop\n");
        printf("4. View Crops\n");
        printf("5. Go Back\n");
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
                add_crop_for_user(user);
            } break;

            case 2: {
                update_crop_for_user(user);
            } break;

            case 3: {
                delete_crop_for_user(user);
            } break;

            case 4: {
                view_crops_for_user(user);
            } break;

            case 5: {
                return;
            } break;

            default: {
                printf("Invalid choice.\n");
            } break;
        }
    } while (1);
}
