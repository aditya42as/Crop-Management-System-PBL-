// historical_advanced.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "historical.h"
#include "weather.h"

void estimate_advanced_for_user(const char *username) {
    History h;
    memset(&h, 0, sizeof(h));

    printf("\n--- Advanced Yield and Profit Estimation ---\n");
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

    printf("Enter base expected yield per area: ");
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

    load_weather_for_user(username);

    Weather *cur = weather_head;
    int count = 0;
    float sumTemp = 0.0f;
    float sumHum = 0.0f;
    float sumRain = 0.0f;
    while (cur) {
        sumTemp += cur->temperature;
        sumHum += cur->humidity;
        sumRain += cur->rainfall;
        count++;
        cur = cur->next;
    }

    if (count > 0) {
        h.avgTemp = sumTemp / count;
        h.avgHumidity = sumHum / count;
        h.totalRainfall = sumRain;
    } else {
        h.avgTemp = h.avgHumidity = h.totalRainfall = 0.0f;
    }

    float factor = 1.0f;
    if (h.avgTemp > 35.0f) {
        factor -= 0.10f;
    }
    if (h.avgTemp < 10.0f) {
        factor -= 0.10f;
    }
    if (h.avgHumidity < 30.0f) {
        factor -= 0.05f;
    }
    if (h.totalRainfall > (20.0f * (h.area + 1.0f))) {
        factor += 0.05f;
    }

    h.yieldPerArea *= factor;
    h.totalYield = h.area * h.yieldPerArea;
    h.revenue = h.totalYield * h.marketPrice;
    h.profit = h.revenue - h.cost;

    merge_and_save_record(username, h);

    printf("\n--- Advanced Estimation Complete ---\n");
    printf("Avg Temp: %.2f, Avg Humidity: %.2f, Total Rainfall: %.2f\n",
           h.avgTemp, h.avgHumidity, h.totalRainfall);
    printf("Adjusted Yield/Area: %.2f\n", h.yieldPerArea);
    printf("Total Yield: %.2f units\n", h.totalYield);
    printf("Total Revenue: ₹%.2f\n", h.revenue);
    printf("Profit: ₹%.2f\n", h.profit);
}
