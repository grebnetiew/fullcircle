#include "storage.h"

void load_palette(Palette *p) {
  *p = (Palette) {
      COLOR_FALLBACK(persist_exists(KEY_COL_APPOINTMENT) ? persist_read_gcolor(KEY_COL_APPOINTMENT) : GColorWhite,       GColorWhite),
      COLOR_FALLBACK(persist_exists(KEY_COL_MINUTE)      ? persist_read_gcolor(KEY_COL_MINUTE)      : GColorTiffanyBlue, GColorWhite),
      COLOR_FALLBACK(persist_exists(KEY_COL_HOUR)        ? persist_read_gcolor(KEY_COL_HOUR)        : GColorRed,         GColorWhite),
      COLOR_FALLBACK(persist_exists(KEY_COL_BACKGROUND)  ? persist_read_gcolor(KEY_COL_BACKGROUND)  : GColorBlack,       GColorBlack),
      COLOR_FALLBACK(persist_exists(KEY_COL_CIRCLE)      ? persist_read_gcolor(KEY_COL_CIRCLE)      : GColorRed,         GColorWhite)
  };
}

void save_palette(Palette *p) {
  persist_write_gcolor(KEY_COL_APPOINTMENT, p->appointments);
  persist_write_gcolor(KEY_COL_MINUTE,      p->minutes);
  persist_write_gcolor(KEY_COL_HOUR,        p->hours);
  persist_write_gcolor(KEY_COL_BACKGROUND,  p->background);
  persist_write_gcolor(KEY_COL_CIRCLE,      p->circle);
}

extern Palette *s_palette;
void set_palette(uint32_t key, GColor col) {
  switch (key - 10) {
    case KEY_COL_HOUR:        s_palette->hours = col;        break;
    case KEY_COL_MINUTE:      s_palette->minutes = col;      break;
    case KEY_COL_APPOINTMENT: s_palette->appointments = col; break;
    case KEY_COL_CIRCLE:      s_palette->circle = col;       break;
    case KEY_COL_BACKGROUND:  s_palette->background = col;   break;
  }
}