#include <pebble.h>
#include "ui.h"

// Define struct to store colors for each time unit
typedef struct Palette {
  GColor appointments;
  GColor minutes;
  GColor hours;
} Palette;

GRect s_screen;

static Window *s_window;
static Layer *s_layer;
static Palette *s_palette;

static uint8_t s_hour;
static uint8_t s_minute;

// Update the watchface display
static void update_display(Layer *layer, GContext *ctx) {
  graphics_context_set_stroke_width(ctx, LINE_THICKNESS);
  graphics_context_set_stroke_color(ctx, s_palette->minutes);
  draw_minutes(ctx, s_minute);
  
  graphics_context_set_stroke_color(ctx, s_palette->hours);
  draw_hours(ctx, s_hour % 12, s_minute);
  
  graphics_context_set_stroke_color(ctx, s_palette->appointments);
  draw_appointments(ctx);
}

// Update the current time values for the watchface
static void update_time(struct tm *tick_time) {
  s_hour = tick_time->tm_hour;
  s_minute = tick_time->tm_min;
  layer_mark_dirty(s_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time(tick_time);
}

static void window_load(Window *window) {
  s_palette = malloc(sizeof(Palette));
  *s_palette = (Palette) {
      COLOR_FALLBACK(GColorOrange, GColorWhite),
      COLOR_FALLBACK(GColorTiffanyBlue, GColorWhite),
      COLOR_FALLBACK(GColorRed, GColorWhite)
  };

  s_layer = layer_create(layer_get_bounds(window_get_root_layer(s_window)));
  s_screen = grect_inset(layer_get_bounds(s_layer), GEdgeInsets(SCREEN_BORDER));
  layer_add_child(window_get_root_layer(s_window), s_layer);
  layer_set_update_proc(s_layer, update_display);
}

static void window_unload(Window *window) {
  free(s_palette);
  layer_destroy(s_layer);
}

static void init(void) {
  s_window = window_create();

  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload
  });
  window_set_background_color(s_window, GColorBlack);
  window_stack_push(s_window, true);

  time_t start = time(NULL);
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  update_time(localtime(&start));
}

static void deinit(void) {
  tick_timer_service_unsubscribe();
  window_destroy(s_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
