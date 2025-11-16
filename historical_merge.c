// historical_merge.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "historical.h"



void merge_and_save_record(const char *username, History h) {
    History *node = (History *)malloc(sizeof(History));
    if (!node) {
        printf("Memory allocation failed.\n");
        return;
    }
    *node = h;
    node->next = history_head;
    history_head = node;

    save_history_for_user(username);
}

void save_history_for_user(const char *username) {
    char fname[200];
    snprintf(fname, sizeof(fname), "%s_combined_history.txt", username);

    FILE *fp = fopen(fname, "w");
    if (!fp) {
        printf("Error saving historical data.\n");
        return;
    }

    History *cur = history_head;
    while (cur) {
        fprintf(fp, "%s|%s|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f\n",
                cur->cropName, cur->harvestDate, cur->area, cur->yieldPerArea,
                cur->totalYield, cur->marketPrice, cur->revenue, cur->cost,
                cur->profit, cur->avgTemp, cur->avgHumidity, cur->totalRainfall);
        cur = cur->next;
    }

    if (fclose(fp) != 0) {
        // ignore
    }
}

void load_history_for_user(const char *username) {
    History *t;
    while (history_head) {
        t = history_head;
        history_head = history_head->next;
        free(t);
    }

    char fname[200];
    snprintf(fname, sizeof(fname), "%s_combined_history.txt", username);

    FILE *fp = fopen(fname, "r");
    if (!fp) {
        return;
    }

    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = '\0';
        History temp;
        char *tok = strtok(line, "|");
        if (!tok) {
            continue;
        }
        strncpy(temp.cropName, tok, sizeof(temp.cropName) - 1);
        temp.cropName[sizeof(temp.cropName) - 1] = '\0';

        tok = strtok(NULL, "|");
        if (!tok) {
            continue;
        }
        strncpy(temp.harvestDate, tok, sizeof(temp.harvestDate) - 1);
        temp.harvestDate[sizeof(temp.harvestDate) - 1] = '\0';

        tok = strtok(NULL, "|");
        if (!tok) {
            continue;
        }
        temp.area = atof(tok);

        tok = strtok(NULL, "|");
        if (!tok) {
            continue;
        }
        temp.yieldPerArea = atof(tok);

        tok = strtok(NULL, "|");
        if (!tok) {
            continue;
        }
        temp.totalYield = atof(tok);

        tok = strtok(NULL, "|");
        if (!tok) {
            continue;
        }
        temp.marketPrice = atof(tok);

        tok = strtok(NULL, "|");
        if (!tok) {
            continue;
        }
        temp.revenue = atof(tok);

        tok = strtok(NULL, "|");
        if (!tok) {
            continue;
        }
        temp.cost = atof(tok);

        tok = strtok(NULL, "|");
        if (!tok) {
            continue;
        }
        temp.profit = atof(tok);

        tok = strtok(NULL, "|");
        if (!tok) {
            continue;
        }
        temp.avgTemp = atof(tok);

        tok = strtok(NULL, "|");
        if (!tok) {
            continue;
        }
        temp.avgHumidity = atof(tok);

        tok = strtok(NULL, "|");
        if (!tok) {
            continue;
        }
        temp.totalRainfall = atof(tok);

        History *node = (History *)malloc(sizeof(History));
        if (!node) {
            continue;
        }
        *node = temp;
        node->next = history_head;
        history_head = node;
    }

    if (fclose(fp) != 0) {
        // ignore
    }
}

void view_combined_history(const char *username) {
    load_history_for_user(username);

    if (!history_head) {
        printf("No historical records.\n");
        return;
    }

    printf("\n%-15s %-12s %-6s %-8s %-10s %-8s %-8s %-8s\n",
           "Crop", "HarvestDate", "Area", "Yld/Area", "TotYield", "Price", "Revenue", "Profit");
    printf("--------------------------------------------------------------------------------\n");

    History *cur = history_head;
    while (cur) {
        printf("%-15s %-12s %-6.2f %-8.2f %-10.2f %-8.2f %-8.2f %-8.2f\n",
               cur->cropName, cur->harvestDate, cur->area, cur->yieldPerArea,
               cur->totalYield, cur->marketPrice, cur->revenue, cur->profit);
        cur = cur->next;
    }
}
