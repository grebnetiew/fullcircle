#include <pebble.h>
#include "drawing.h"
#include "storage.h"

static Window *s_window;
GRect s_screen;

Layer *s_layer;
TextLayer *s_date_text;

extern Palette *s_palette;

static uint8_t s_hour;
static uint8_t s_minute;
static char s_date[8];
GRect s_date_positions[3];
bool s_connected;

// Update the watchface display
static void update_display(Layer *layer, GContext *ctx) {
  window_set_background_color(s_window, s_palette->background);
  
  if (gcolor_equal(s_palette->circle, s_palette->appointments)) {
    // A nonsense setting used to encode "draw me a pie chart"
    draw_piechart_appointments(ctx);
  } else {
    draw_fullcircle(ctx);
    draw_appointments(ctx);
  }
  
  draw_date(s_date, s_hour % 12, s_minute);
  
  // Draw clock
  draw_minutes(ctx, s_minute);
  draw_hours(ctx, s_hour % 12, s_minute);
}

// Update the display if time or connection status changes
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  s_hour = tick_time->tm_hour;
  s_minute = tick_time->tm_min;
  strftime(s_date, sizeof(s_date), "%a %e", tick_time);
  layer_mark_dirty(s_layer);
}

static void bluetooth_handler(bool connected) {
  s_connected = connected;
  layer_mark_dirty(s_layer);
}

static void window_load(Window *window) {
  s_palette = malloc(sizeof(Palette));
  load_palette(s_palette);
  appsync_init();

  s_layer = layer_create(layer_get_bounds(window_get_root_layer(s_window)));
  s_screen = grect_inset(layer_get_bounds(s_layer), GEdgeInsets(SCREEN_BORDER));
  layer_add_child(window_get_root_layer(s_window), s_layer);
  layer_set_update_proc(s_layer, update_display);
  
  GPoint center = grect_center_point(&s_screen);
  // Make three possible locations for the date
  s_date_positions[0] = GRect(center.x + 15, center.y - 9,  60, 18);
  s_date_positions[1] = GRect(center.x - 30, center.y + 15, 60, 18);
  s_date_positions[2] = GRect(center.x - 30, center.y - 33, 60, 18);
  
  s_date_text = text_layer_create(GRect(center.x + 15, center.y - 9, 60, 18));
  layer_add_child(s_layer, text_layer_get_layer(s_date_text));
}

static void window_unload(Window *window) {
  save_calendar();
  save_palette(s_palette);
  free(s_palette);
  text_layer_destroy(s_date_text);
  layer_destroy(s_layer);
}

static void init() {
  s_window = window_create();
  s_connected = connection_service_peek_pebble_app_connection();

  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload
  });
  window_set_background_color(s_window, GColorBlack);
  window_stack_push(s_window, true);

  time_t start = time(NULL);
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  connection_service_subscribe((ConnectionHandlers) {
    .pebble_app_connection_handler = bluetooth_handler
  });
  
  tick_handler(localtime(&start), 0);
}

static void deinit() {
  appsync_deinit();
  tick_timer_service_unsubscribe();
  window_destroy(s_window);
}

int main() {
  init();
  app_event_loop();
  deinit();
}
