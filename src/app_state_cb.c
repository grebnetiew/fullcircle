#include "app_state_private.h"
#include "app_data.h"

void appstate_cb_load(AppState *as, Window *window) {
  appstate_window_init(as);
  appdata_init(as->appData, as);
}

void appstate_cb_unload(AppState *as, Window *window) {
  appdata_deinit(as->appData);
  appstate_window_deinit(as);
}

void appstate_cb_tick(AppState *as, struct tm *tick_time, TimeUnits units_changed) {
  as->hour = tick_time->tm_hour;
  as->minute = tick_time->tm_min;
  strftime(as->date, sizeof(as->date), "%a %e", tick_time);
  layer_mark_dirty(as->layer);
}

void appstate_cb_bluetooth(AppState *as, bool connected) {
  as->connected = connected;
  layer_mark_dirty(as->layer);
}
