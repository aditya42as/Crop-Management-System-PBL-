// crop.h
#ifndef CROP_H
#define CROP_H

#define CROP_NAME_LEN 50
#define CROP_TYPE_LEN 30
#define CROP_SEASON_LEN 20
#define CROP_HARVEST_LEN 20
#define CROP_WATER_LEN 20
#define CROP_FERT_LEN 100

typedef struct Crop {
    char name[CROP_NAME_LEN];
    char type[CROP_TYPE_LEN];
    char season[CROP_SEASON_LEN];
    char harvest[CROP_HARVEST_LEN];
    char waterReq[CROP_WATER_LEN];
    char fertilizer[CROP_FERT_LEN];
    float price;
    float area;
    struct Crop *next;
} Crop;

extern Crop *crop_head;

// user-specific function prototypes
void load_crops_for_user(const char *username);
void save_crops_for_user(const char *username);
void add_crop_for_user(const char *username);
void update_crop_for_user(const char *username);
void delete_crop_for_user(const char *username);
void view_crops_for_user(const char *username);

#endif
