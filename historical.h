// historical.h
#ifndef HISTORICAL_H
#define HISTORICAL_H

typedef struct History {
    char cropName[50];
    char harvestDate[20];
    float area;
    float yieldPerArea;
    float totalYield;
    float marketPrice;
    float revenue;
    float cost;
    float profit;
    float avgTemp;
    float avgHumidity;
    float totalRainfall;
    struct History *next;
} History;

extern History *history_head;

void historical_manage();
void estimate_basic_for_user(const char *username);
void estimate_advanced_for_user(const char *username);
void merge_and_save_record(const char *username, History h);
void view_combined_history(const char *username);
void load_history_for_user(const char *username);
void save_history_for_user(const char *username);

#endif
