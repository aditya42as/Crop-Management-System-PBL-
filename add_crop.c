// add_crop.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crop.h"

void add_crop_for_user(const char *username) {
    load_crops_for_user(username);

    Crop *newCrop = (Crop *)malloc(sizeof(Crop));
    if (!newCrop) {
        printf("Memory allocation failed.\n");
        return;
    }

    printf("\n=== ADD NEW CROP ===\n");
    printf("Crop Name: ");
    if (fgets(newCrop->name, sizeof(newCrop->name), stdin) != NULL) {
        newCrop->name[strcspn(newCrop->name, "\n")] = '\0';
    } else {
        newCrop->name[0] = '\0';
    }

    printf("Type (Cereal, Vegetable, etc): ");
    if (fgets(newCrop->type, sizeof(newCrop->type), stdin) != NULL) {
        newCrop->type[strcspn(newCrop->type, "\n")] = '\0';
    } else {
        newCrop->type[0] = '\0';
    }

    printf("Season: ");
    if (fgets(newCrop->season, sizeof(newCrop->season), stdin) != NULL) {
        newCrop->season[strcspn(newCrop->season, "\n")] = '\0';
    } else {
        newCrop->season[0] = '\0';
    }

    printf("Harvest Month: ");
    if (fgets(newCrop->harvest, sizeof(newCrop->harvest), stdin) != NULL) {
        newCrop->harvest[strcspn(newCrop->harvest, "\n")] = '\0';
    } else {
        newCrop->harvest[0] = '\0';
    }

    printf("Water Requirement (High/Medium/Low): ");
    if (fgets(newCrop->waterReq, sizeof(newCrop->waterReq), stdin) != NULL) {
        newCrop->waterReq[strcspn(newCrop->waterReq, "\n")] = '\0';
    } else {
        newCrop->waterReq[0] = '\0';
    }

    printf("Fertilizer Used: ");
    if (fgets(newCrop->fertilizer, sizeof(newCrop->fertilizer), stdin) != NULL) {
        newCrop->fertilizer[strcspn(newCrop->fertilizer, "\n")] = '\0';
    } else {
        newCrop->fertilizer[0] = '\0';
    }

    printf("Market Price (INR): ");
    if (scanf("%f", &newCrop->price) != 1) {
        newCrop->price = 0.0f;
    }
    getchar();

    printf("Area (acres): ");
    if (scanf("%f", &newCrop->area) != 1) {
        newCrop->area = 0.0f;
    }
    getchar();

    newCrop->next = crop_head;
    crop_head = newCrop;

    save_crops_for_user(username);
    printf("\nCrop added successfully!\n");
}
