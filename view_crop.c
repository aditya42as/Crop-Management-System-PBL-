// view_crop.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crop.h"


void view_crops_for_user(const char *username) {
    load_crops_for_user(username);

    printf("\n============================== CROP LIST ===============================\n");

    if (!crop_head) {
        printf("No crops recorded.\n");
        return;
    }

    printf("%-3s %-20s %-12s %-10s %-12s %-12s %-8s %-8s\n",
           "No", "Name", "Type", "Season", "Harvest", "Water", "Price", "Area");
    printf("-------------------------------------------------------------------------------\n");

    Crop *temp = crop_head;
    int i = 1;
    while (temp) {
        printf("%-3d %-20s %-12s %-10s %-12s %-12s %-8.2f %-8.2f\n",
               i++, temp->name, temp->type, temp->season, temp->harvest,
               temp->waterReq, temp->price, temp->area);
        temp = temp->next;
    }

    printf("-------------------------------------------------------------------------------\n");
}
