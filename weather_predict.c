// weather_predict.c
// Simple weather prediction based on last available weather records.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "weather.h"   

//  compute simple average
static float avg(float *arr, int n)
{
    float s = 0.0f;
    int i;
    for (i = 0; i < n; i++)
    {
        s += arr[i];
    }
    return (n == 0) ? 0.0f : s / n;
}

// Linear regression: y = a + b*x for x = 0..n-1
static void linear_predict(float *arr, int n, float *next_val)
{
    if (n < 2)
    {
        *next_val = (n == 1) ? arr[0] : 0.0f;
        return;
    }

    float x_mean = (float)(n - 1) / 2.0f;
    float y_mean = avg(arr, n);

    float num = 0.0f;
    float den = 0.0f;

    for (int i = 0; i < n; i++)
    {
        num += (i - x_mean) * (arr[i] - y_mean);
        den += (i - x_mean) * (i - x_mean);
    }

    float b = (den == 0.0f) ? 0.0f : (num / den);
    float a = y_mean - b * x_mean;

    int next_x = n;             
    *next_val = a + b * next_x;
}


void weather_predict_for_user()
{
    if (weather_head == NULL)
    {
        printf("\nNo weather data available.\n");
        return;
    }

    int count = 0;

    Weather *temp = weather_head;
    while (temp != NULL)
    {
        count++;
        temp = temp->next;
    }

    if (count == 0)
    {
        printf("\nNo weather records found.\n");
        return;
    }

    float *temps = malloc(sizeof(float) * count);
    float *hums  = malloc(sizeof(float) * count);
    float *rains = malloc(sizeof(float) * count);

    if (!temps || !hums || !rains)
    {
        printf("Memory allocation failed.\n");
        return;
    }

    temp = weather_head;
    int i = 0;
    while (temp != NULL)
    {
        temps[i] = temp->temperature;
        hums[i]  = temp->humidity;
        rains[i] = temp->rainfall;
        i++;
        temp = temp->next;
    }

   
    float t_pred, h_pred, r_pred;

    if (count >= 3)
    {
        linear_predict(temps, count, &t_pred);
        linear_predict(hums, count, &h_pred);
        linear_predict(rains, count, &r_pred);
    }
    else
    {
        
        t_pred = avg(temps, count);
        h_pred = avg(hums, count);
        r_pred = avg(rains, count);
    }

    
    free(temps);
    free(hums);
    free(rains);

   
    if (t_pred < -50) t_pred = -50;
    if (t_pred > 60)  t_pred = 60;

    if (h_pred < 0)   h_pred = 0;
    if (h_pred > 100) h_pred = 100;

    if (r_pred < 0)   r_pred = 0;

    
    printf("\n=== Weather Prediction (C Program) ===\n");
    printf("Next Temperature: %.2f °C\n", t_pred);
    printf("Next Humidity:    %.2f %%\n", h_pred);
    printf("Next Rainfall:    %.2f mm\n", r_pred);
}
