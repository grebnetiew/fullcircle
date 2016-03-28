#include <pebble.h>
#include "palette.h"
#include "storage.h"
#include "app_state.h"
#include "globals.h"

extern AppState appState;

GColor maybe_to_gray(GColor col) {
  if (appState.connected) {
    return col;
  }
  // col.r, g, b are each between 0 and 3
  uint8_t average = (col.r + col.g + col.b) / 3;
  return (GColor)(uint8_t)(0b11000000 + average + (average << 2) + (average << 4));
}

void set(Palette *p, uint32_t key, GColor col) {
  switch (key - 10) {
    case KEY_COL_HOUR:        p->hours = col;        break;
    case KEY_COL_MINUTE:      p->minutes = col;      break;
    case KEY_COL_APPOINTMENT: p->appointments = col; break;
    case KEY_COL_CIRCLE:      p->circle = col;       break;
    case KEY_COL_BACKGROUND:  p->background = col;   break;
    case KEY_COL_DATE:        p->date = col;         break;
  }
}