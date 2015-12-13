#include "storage.h"

void load_palette(Palette *p) {
  *p = (Palette) {
      COLOR_FALLBACK(persist_exists(APPOINTMENT_COLOR_KEY) ? persist_read_gcolor(APPOINTMENT_COLOR_KEY) : GColorWhite,       GColorWhite),
      COLOR_FALLBACK(persist_exists(MINUTE_COLOR_KEY)      ? persist_read_gcolor(MINUTE_COLOR_KEY)      : GColorTiffanyBlue, GColorWhite),
      COLOR_FALLBACK(persist_exists(HOUR_COLOR_KEY)        ? persist_read_gcolor(HOUR_COLOR_KEY)        : GColorRed,         GColorWhite),
      COLOR_FALLBACK(persist_exists(BACKGROUND_COLOR_KEY)  ? persist_read_gcolor(BACKGROUND_COLOR_KEY)  : GColorBlack,       GColorBlack)
  };
}

void save_palette(Palette *p) {
  persist_write_gcolor(APPOINTMENT_COLOR_KEY, p->appointments);
  persist_write_gcolor(MINUTE_COLOR_KEY,      p->minutes);
  persist_write_gcolor(HOUR_COLOR_KEY,        p->hours);
  persist_write_gcolor(BACKGROUND_COLOR_KEY,  p->background);
}