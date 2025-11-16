// weather.h
#ifndef WEATHER_H
#define WEATHER_H

#define DATE_LEN 20

typedef struct Weather {
    char date[DATE_LEN]; // DD-MM-YYYY or YYYY-MM-DD
    float temperature;
    float humidity;
    float rainfall;
    struct Weather *next;
} Weather;

extern Weather *weather_head;

// prototypes
void load_weather_for_user(const char *username);
void save_weather_for_user(const char *username);
void weather_manage_menu();
void add_weather_for_user(const char *username);
void view_weather_for_user(const char *username);
void update_weather_for_user(const char *username);
void delete_weather_for_user(const char *username);
void weather_predict_for_user(const char *username);

#endif
