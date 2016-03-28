#pragma once
#include <pebble.h>
#include "palette.h"

/*
  This struct aims to contain all info about the app content
  (calendar, palette, etc) and offer ways to manipulate it.
  In this way, it replaces the old and interlocked AppSync,
  Storage and Palette structs/methods.
*/

#define CAL_CURRENT_VER 1
#define CAL_LENGTH      10  // appointments
#define CAL_BUFFER_SIZE 128 // bytes
/* Size calc: 114
   Dictionary hdr 1 (1)
   Version tuple: 7 header + 4 data (11)
   Storage: 40 bytes + 7 hdr (47)
   Colors: 5 tuples, each 7 hdr and 4 data. (55)
*/

typedef struct str_appstate AppState;

typedef struct str_appdata {
  AppState *appState; // To be able to signal a redraw
  Palette palette;    // Color data for the watchface
  AppSync appSync;    // Calendar storage and comms
  uint8_t sync_buffer[CAL_BUFFER_SIZE];
} AppData;

// "Member" functions of AppData
void appdata_init(AppData *ad, AppState *as);
void appdata_deinit(AppData *ad);

  // Both these functions return time in minutes after 12
uint32_t appointment_start(AppData *ad, uint8_t idx);
  // The end of an appointment is always after the start, possibly > 12 * 60
uint32_t appointment_end(AppData *ad, uint8_t idx);

inline uint32_t hexFromGColor(GColor c) {
  return 0x550000 * c.r + 0x5500 * c.g + 0x55 * c.b;
}