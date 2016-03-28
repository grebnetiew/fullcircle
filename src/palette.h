#pragma once
#include <pebble.h>

typedef struct str_palette {
  GColor appointments;
  GColor minutes;
  GColor hours;
  GColor background;
  GColor circle;
  GColor date;
} Palette;

void set(Palette *p, uint32_t key, GColor col);

GColor maybe_to_gray(GColor col);