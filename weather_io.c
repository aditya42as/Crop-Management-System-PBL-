// weather_io.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "weather.h"

static void free_weather_list() {
    Weather *t;
    while (weather_head) {
        t = weather_head;
        weather_head = weather_head->next;
        free(t);
    }
}

void load_weather_for_user(const char *username) {
    free_weather_list();

    char fname[200];
    snprintf(fname, sizeof(fname), "%s_weather.txt", username);

    FILE *fp = fopen(fname, "r");
    if (!fp) {
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = '\0';
        Weather temp;
        char *tok = strtok(line, "|");
        if (!tok) {
            continue;
        }
        strncpy(temp.date, tok, sizeof(temp.date) - 1);
        temp.date[sizeof(temp.date) - 1] = '\0';

        tok = strtok(NULL, "|");
        if (!tok) {
            continue;
        }
        temp.temperature = atof(tok);

        tok = strtok(NULL, "|");
        if (!tok) {
            continue;
        }
        temp.humidity = atof(tok);

        tok = strtok(NULL, "|");
        if (!tok) {
            continue;
        }
        temp.rainfall = atof(tok);

        Weather *node = (Weather *)malloc(sizeof(Weather));
        if (!node) {
            continue;
        }
        *node = temp;
        node->next = weather_head;
        weather_head = node;
    }

    if (fclose(fp) != 0) {
        // ignore
    }
}

void save_weather_for_user(const char *username) {
    char fname[200];
    snprintf(fname, sizeof(fname), "%s_weather.txt", username);

    FILE *fp = fopen(fname, "w");
    if (!fp) {
        printf("Error saving weather data.\n");
        return;
    }

    Weather *cur = weather_head;
    while (cur) {
        fprintf(fp, "%s|%.2f|%.2f|%.2f\n",
                cur->date, cur->temperature, cur->humidity, cur->rainfall);
        cur = cur->next;
    }

    if (fclose(fp) != 0) {
        // ignore
    }
}
