#include "storage.h"
#include "appsync.h"

void load_palette(Palette *p) {
  *p = (Palette) {
      COLOR_FALLBACK(persist_exists(KEY_COL_APPOINTMENT) ? persist_read_gcolor(KEY_COL_APPOINTMENT) : GColorBlack,      GColorBlack),
      COLOR_FALLBACK(persist_exists(KEY_COL_MINUTE)      ? persist_read_gcolor(KEY_COL_MINUTE)      : GColorWhite,      GColorWhite),
      COLOR_FALLBACK(persist_exists(KEY_COL_HOUR)        ? persist_read_gcolor(KEY_COL_HOUR)        : GColorWindsorTan, GColorWhite),
      COLOR_FALLBACK(persist_exists(KEY_COL_BACKGROUND)  ? persist_read_gcolor(KEY_COL_BACKGROUND)  : GColorBlack,      GColorBlack),
      COLOR_FALLBACK(persist_exists(KEY_COL_CIRCLE)      ? persist_read_gcolor(KEY_COL_CIRCLE)      : GColorWindsorTan, GColorWhite),
      COLOR_FALLBACK(persist_exists(KEY_COL_DATE)        ? persist_read_gcolor(KEY_COL_DATE)        : GColorWhite,      GColorWhite)
  };
}

void save_palette(Palette *p) {
  persist_write_gcolor(KEY_COL_APPOINTMENT, p->appointments);
  persist_write_gcolor(KEY_COL_MINUTE,      p->minutes);
  persist_write_gcolor(KEY_COL_HOUR,        p->hours);
  persist_write_gcolor(KEY_COL_BACKGROUND,  p->background);
  persist_write_gcolor(KEY_COL_CIRCLE,      p->circle);
  persist_write_gcolor(KEY_COL_DATE,        p->date);
}

void load_calendar(uint8_t* cal, uint8_t length) {
  if (!persist_exists(KEY_CALENDAR))
    return;
  persist_read_data(KEY_CALENDAR, cal, length);
  // Any update which results in higher length will have to
  // be careful about read overflow...
}

extern AppSync s_sync;
extern uint8_t DATA_LENGTH;

void save_calendar() {
  // Read calendar data from appsync cache
  const Tuple *cal = app_sync_get(&s_sync, CAL_DATA_KEY);
  // Write result
  persist_write_data(KEY_CALENDAR, cal->value, DATA_LENGTH);
}
