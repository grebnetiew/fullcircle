#pragma once
#include <pebble.h>

typedef struct str_appdata AppData;

typedef struct str_appstate {
  AppData *appData;
  
  Window *window;
  GRect screen;
  Layer *layer;
  
  TextLayer *date_text;
  GRect date_positions[3];

  uint8_t hour;
  uint8_t minute;
  char date[8];
  bool connected;
} AppState;

// "Member" functions of AppState
void appstate_init(AppState *as, AppData *ad);
void appstate_deinit(AppState *as);