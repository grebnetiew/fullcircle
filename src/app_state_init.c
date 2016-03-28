#include "app_state_private.h"

static AppState *appStateForCallbacks;
void window_load(Window *window) {
  appstate_cb_load(appStateForCallbacks, window);
}
void window_unload(Window *window) {
  appstate_cb_unload(appStateForCallbacks, window);
}
void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  appstate_cb_tick(appStateForCallbacks, tick_time, units_changed);
}
void bluetooth_handler(bool connected) {
  appstate_cb_bluetooth(appStateForCallbacks, connected);
}

void appstate_init(AppState *as, AppData *ad) {
  appStateForCallbacks = as;
  as->appData = ad;
  
  as->window = window_create();
  as->connected = connection_service_peek_pebble_app_connection();

  window_set_window_handlers(as->window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload
  });
  window_set_background_color(as->window, GColorBlack);
  window_stack_push(as->window, true);

  time_t start = time(NULL);
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  connection_service_subscribe((ConnectionHandlers) {
    .pebble_app_connection_handler = bluetooth_handler
  });
  
  tick_handler(localtime(&start), 0);
}

void appstate_deinit(AppState *as) {
  tick_timer_service_unsubscribe();
  window_destroy(as->window);
}