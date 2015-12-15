#include <pebble.h>
#include "palette.h"
#include "storage.h"

extern bool s_connected;

Palette *s_palette;

#ifdef PBL_COLOR
GColor maybe_to_gray(GColor col) {
  if (s_connected) {
    return col;
  }
  // col.r, g, b are each between 0 and 3
  uint8_t average = (col.r + col.g + col.b) / 3;
  return (GColor)(uint8_t)(0b11000000 + average + (average << 2) + (average << 4));
}
#else
GColor maybe_to_gray(GColor col) {
  return col; // There is no effective grayscale on Aplite
}
#endif

void set_palette(uint32_t key, GColor col) {
  switch (key - 10) {
    case KEY_COL_HOUR:        s_palette->hours = col;        break;
    case KEY_COL_MINUTE:      s_palette->minutes = col;      break;
    case KEY_COL_APPOINTMENT: s_palette->appointments = col; break;
    case KEY_COL_CIRCLE:      s_palette->circle = col;       break;
    case KEY_COL_BACKGROUND:  s_palette->background = col;   break;
    case KEY_COL_DATE:        s_palette->date = col;         break;
  }
}