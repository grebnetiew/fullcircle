#pragma once

#include <pebble.h>

typedef struct Palette {
  GColor appointments;
  GColor minutes;
  GColor hours;
  GColor background;
  GColor circle;
} Palette;

enum PersistKeys {
  APPOINTMENT_COLOR_KEY = 1,
  MINUTE_COLOR_KEY = 2,
  HOUR_COLOR_KEY = 3,
  BACKGROUND_COLOR_KEY = 4,
  CIRCLE_COLOR_KEY = 5,
};

inline GColor8 persist_read_gcolor(int32_t key) {
  return (GColor8) (uint8_t) persist_read_int(key);
}

inline status_t persist_write_gcolor(uint32_t key, GColor8 value) {
  return persist_write_int(key, value.argb);
}

void load_palette(Palette *p);
void save_palette(Palette *p);