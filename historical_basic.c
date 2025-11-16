// historical_basic.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "historical.h"

void estimate_basic_for_user(const char *username) {
    History h;
    memset(&h, 0, sizeof(h));

    printf("\n--- Basic Yield and Profit Estimation ---\n");
    printf("Enter crop name: ");
    if (fgets(h.cropName, sizeof(h.cropName), stdin) != NULL) {
        h.cropName[strcspn(h.cropName, "\n")] = '\0';
    } else {
        h.cropName[0] = '\0';
    }

    printf("Enter date of harvest (YYYY-MM-DD): ");
    if (fgets(h.harvestDate, sizeof(h.harvestDate), stdin) != NULL) {
        h.harvestDate[strcspn(h.harvestDate, "\n")] = '\0';
    } else {
        h.harvestDate[0] = '\0';
    }

    printf("Enter cultivated area (in acres): ");
    if (scanf("%f", &h.area) != 1) {
        h.area = 0.0f;
    }
    getchar();

    printf("Enter expected yield per area: ");
    if (scanf("%f", &h.yieldPerArea) != 1) {
        h.yieldPerArea = 0.0f;
    }
    getchar();

    printf("Enter market price per unit: ");
    if (scanf("%f", &h.marketPrice) != 1) {
        h.marketPrice = 0.0f;
    }
    getchar();

    printf("Enter total cost of cultivation: ");
    if (scanf("%f", &h.cost) != 1) {
        h.cost = 0.0f;
    }
    getchar();

    h.totalYield = h.area * h.yieldPerArea;
    h.revenue = h.totalYield * h.marketPrice;
    h.profit = h.revenue - h.cost;
    h.avgTemp = h.avgHumidity = h.totalRainfall = 0.0f;

    merge_and_save_record(username, h);

    printf("\n--- Estimation Complete ---\n");
    printf("Total Yield: %.2f units\n", h.totalYield);
    printf("Total Revenue: ₹%.2f\n", h.revenue);
    printf("Profit: ₹%.2f\n", h.profit);
}
