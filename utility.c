// utility.c
#include <stdio.h>
#include <string.h>

// remove trailing newline if present
void remove_newline(char *str) {
    int len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

// read integer choice robustly
int int_choice() {
    int choice;
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n') {
            // consume
        }
        return -1;
    }
    getchar();
    return choice;
}
