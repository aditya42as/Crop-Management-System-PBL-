// update_crop.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crop.h"

static Crop *find_crop_by_name(const char *name) {
    Crop *cur = crop_head;
    while (cur) {
        if (strcmp(cur->name, name) == 0) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

void update_crop_for_user(const char *username) {
    load_crops_for_user(username);

    if (!crop_head) {
        printf("No crops to update.\n");
        return;
    }

    char name[CROP_NAME_LEN];
    printf("Enter crop name to update: ");
    if (fgets(name, sizeof(name), stdin) != NULL) {
        name[strcspn(name, "\n")] = '\0';
    } else {
        name[0] = '\0';
    }

    Crop *c = find_crop_by_name(name);
    if (!c) {
        printf("Crop not found.\n");
        return;
    }

    printf("\nEditing %s (leave blank to keep current)\n", c->name);

    char buffer[200];

    printf("New Type (current: %s): ", c->type);
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strlen(buffer) > 0) {
            strncpy(c->type, buffer, sizeof(c->type) - 1);
            c->type[sizeof(c->type) - 1] = '\0';
        }
    }

    printf("New Season (current: %s): ", c->season);
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strlen(buffer) > 0) {
            strncpy(c->season, buffer, sizeof(c->season) - 1);
            c->season[sizeof(c->season) - 1] = '\0';
        }
    }

    printf("New Harvest (current: %s): ", c->harvest);
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strlen(buffer) > 0) {
            strncpy(c->harvest, buffer, sizeof(c->harvest) - 1);
            c->harvest[sizeof(c->harvest) - 1] = '\0';
        }
    }

    printf("New WaterReq (current: %s): ", c->waterReq);
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strlen(buffer) > 0) {
            strncpy(c->waterReq, buffer, sizeof(c->waterReq) - 1);
            c->waterReq[sizeof(c->waterReq) - 1] = '\0';
        }
    }

    printf("New Fertilizer (current: %s): ", c->fertilizer);
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strlen(buffer) > 0) {
            strncpy(c->fertilizer, buffer, sizeof(c->fertilizer) - 1);
            c->fertilizer[sizeof(c->fertilizer) - 1] = '\0';
        }
    }

    printf("New Price (current: %.2f, enter -1 to keep): ", c->price);
    float f;
    if (scanf("%f", &f) == 1) {
        if (f >= 0) {
            c->price = f;
        }
    }
    getchar();

    printf("New Area (current: %.2f, enter -1 to keep): ", c->area);
    if (scanf("%f", &f) == 1) {
        if (f >= 0) {
            c->area = f;
        }
    }
    getchar();

    save_crops_for_user(username);
    printf("Crop updated.\n");
}
