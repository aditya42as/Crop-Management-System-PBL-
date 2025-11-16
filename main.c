// main.c - integrate all modules
#include <stdio.h>
#include <stdlib.h>
#include "auth.c"
#include "crop_manage.c"
#include "weather_manage.c"
#include "historical_manage.c"
#include "todo.c"

// forward declarations
int auth_menu();
void crop_manage_menu();
void weather_manage_menu();
void historical_manage();
void todo_menu();
void logout_user();
const char *get_current_user();

int main_menu() {
    int choice;
    do {
        // attempt cross-platform clear
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        printf("\n=============================\n");
        printf("   CROP MANAGEMENT SYSTEM\n");
        printf("=============================\n");
        printf("1. Crop Management\n");
        printf("2. Weather Tracking\n");
        printf("3. Historical Data\n");
        printf("4. To-Do List\n");
        printf("5. Logout\n");
        printf("-----------------------------\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n') {
                // consume
            }
            choice = 0;
        }
        getchar();

        switch (choice) {
            case 1: {
                crop_manage_menu();
            } break;

            case 2: {
                weather_manage_menu();
            } break;

            case 3: {
                historical_manage();
            } break;

            case 4: {
                todo_menu();
            } break;

            case 5: {
                logout_user();
                printf("Logged out successfully.\n");
                return 0;
            } break;

            default: {
                printf("Invalid choice! Try again.\n");
            } break;
        }
    } while (1);
}

int main() {
    // call auth
    int authenticated = auth_menu();
    if (authenticated) {
        main_menu();
    } else {
        printf("Authentication failed or exited.\n");
    }

    printf("\nThank you for using the system!\n");
    return 0;
}
