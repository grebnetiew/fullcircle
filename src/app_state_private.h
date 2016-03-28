#include "app_state.h"

void appstate_cb_load(AppState *as, Window *window);
void appstate_cb_unload(AppState *as, Window *window);
void appstate_cb_tick(AppState *as, struct tm *tick_time, TimeUnits units_changed);
void appstate_cb_bluetooth(AppState *as, bool connected);

void appstate_window_init(AppState *as);
void appstate_window_deinit(AppState *as);