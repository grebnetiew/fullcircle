#pragma once

#include <pebble.h>
#include "palette.h"

enum PersistKeys {
  KEY_COL_HOUR = 1,
  KEY_COL_MINUTE = 2,
  KEY_COL_APPOINTMENT = 3,
  KEY_COL_CIRCLE = 4,
  KEY_COL_BACKGROUND = 5,
  KEY_COL_DATE = 6,
};

inline GColor8 persist_read_gcolor(int32_t key) {
  return (GColor8) (uint8_t) persist_read_int(key);
}

inline status_t persist_write_gcolor(uint32_t key, GColor8 value) {
  return persist_write_int(key, value.argb);
}

void load_palette(Palette *p);
void save_palette(Palette *p);
