// crop_io.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crop.h"

// free existing list
static void free_crop_list() {
    Crop *t;
    while (crop_head) {
        t = crop_head;
        crop_head = crop_head->next;
        free(t);
    }
}

void load_crops_for_user(const char *username) {
    free_crop_list();

    char fname[200];
    snprintf(fname, sizeof(fname), "%s_Crop_data.txt", username);

    FILE *fp = fopen(fname, "r");
    if (!fp) {
        return;
    }

    // file format: name|type|season|harvest|waterReq|fertilizer|price|area
    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = '\0';
        Crop temp;
        char *tok;

        tok = strtok(line, "|");
        if (!tok) {
            continue;
        }
        strncpy(temp.name, tok, sizeof(temp.name) - 1);
        temp.name[sizeof(temp.name) - 1] = '\0';

        tok = strtok(NULL, "|");
        if (!tok) {
            continue;
        }
        strncpy(temp.type, tok, sizeof(temp.type) - 1);
        temp.type[sizeof(temp.type) - 1] = '\0';

        tok = strtok(NULL, "|");
        if (!tok) {
            continue;
        }
        strncpy(temp.season, tok, sizeof(temp.season) - 1);
        temp.season[sizeof(temp.season) - 1] = '\0';

        tok = strtok(NULL, "|");
        if (!tok) {
            continue;
        }
        strncpy(temp.harvest, tok, sizeof(temp.harvest) - 1);
        temp.harvest[sizeof(temp.harvest) - 1] = '\0';

        tok = strtok(NULL, "|");
        if (!tok) {
            continue;
        }
        strncpy(temp.waterReq, tok, sizeof(temp.waterReq) - 1);
        temp.waterReq[sizeof(temp.waterReq) - 1] = '\0';

        tok = strtok(NULL, "|");
        if (!tok) {
            continue;
        }
        strncpy(temp.fertilizer, tok, sizeof(temp.fertilizer) - 1);
        temp.fertilizer[sizeof(temp.fertilizer) - 1] = '\0';

        tok = strtok(NULL, "|");
        if (!tok) {
            continue;
        }
        temp.price = atof(tok);

        tok = strtok(NULL, "|");
        if (!tok) {
            continue;
        }
        temp.area = atof(tok);

        Crop *node = (Crop *)malloc(sizeof(Crop));
        if (!node) {
            continue;
        }
        *node = temp;
        node->next = crop_head;
        crop_head = node;
    }

    if (fclose(fp) != 0) {
        // ignore
    }
}

void save_crops_for_user(const char *username) {
    char fname[200];
    snprintf(fname, sizeof(fname), "%s_Crop_data.txt", username);

    FILE *fp = fopen(fname, "w");
    if (!fp) {
        printf("Error saving crops to file.\n");
        return;
    }

    Crop *cur = crop_head;
    while (cur) {
        fprintf(fp, "%s|%s|%s|%s|%s|%s|%.2f|%.2f\n",
                cur->name, cur->type, cur->season, cur->harvest,
                cur->waterReq, cur->fertilizer, cur->price, cur->area);
        cur = cur->next;
    }

    if (fclose(fp) != 0) {
        // ignore
    }
}
