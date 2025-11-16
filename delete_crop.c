// delete_crop.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crop.h"

void delete_crop_for_user(const char *username) {
    load_crops_for_user(username);

    if (!crop_head) {
        printf("No crops to delete.\n");
        return;
    }

    char name[CROP_NAME_LEN];
    printf("Enter crop name to delete: ");
    if (fgets(name, sizeof(name), stdin) != NULL) {
        name[strcspn(name, "\n")] = '\0';
    } else {
        name[0] = '\0';
    }

    Crop *cur = crop_head;
    Crop *prev = NULL;
    while (cur) {
        if (strcmp(cur->name, name) == 0) {
            if (prev == NULL) {
                crop_head = cur->next;
            } else {
                prev->next = cur->next;
            }
            free(cur);
            save_crops_for_user(username);
            printf("Crop deleted.\n");
            return;
        }
        prev = cur;
        cur = cur->next;
    }

    printf("Crop not found.\n");
}
