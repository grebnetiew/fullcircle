#pragma once
#include <pebble.h>

typedef struct Palette {
  GColor appointments;
  GColor minutes;
  GColor hours;
  GColor background;
  GColor circle;
} Palette;

void set_palette(uint32_t key, GColor col);

GColor maybe_to_gray(GColor col);