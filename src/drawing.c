 #include <pebble.h>
#include "drawing.h"
#include "storage.h"
#include "palette.h"

extern GRect s_screen;
extern TextLayer *s_date_text;
extern Palette *s_palette;

inline uint32_t angle_from_minutes(uint8_t min) {
  return TRIG_MAX_ANGLE * min / 60;
}

inline uint32_t angle_from_hours(uint32_t min) {
  return TRIG_MAX_ANGLE * min / (12 * 60);
}

inline GPoint GpointFromPolar(GPoint center, uint32_t angle, uint8_t length) {
  return GPoint(
    center.x + length * sin_lookup(angle) / TRIG_MAX_RATIO,
    center.y - length * cos_lookup(angle) / TRIG_MAX_RATIO);
}

void draw_hand(GContext *ctx, uint32_t angle, uint8_t length) {
  // Calculate the endpoints of the lines
  GPoint center = grect_center_point(&s_screen);
  GPoint hand   = GpointFromPolar(center, angle, length);
  GPoint wrist  = GpointFromPolar(center, angle, length / 9);
  
  // Draw hands
  graphics_draw_line(ctx, wrist, hand);
  graphics_draw_circle(ctx, center, length / 10);
}

// Handle representation for minutes
void draw_minutes(GContext *ctx, uint8_t minutes) {
  graphics_context_set_stroke_color(ctx, maybe_to_gray(s_palette->minutes));
  graphics_context_set_stroke_width(ctx, LINE_THICKNESS + 1);
  graphics_context_set_antialiased(ctx, true);
  
  draw_hand(ctx, angle_from_minutes(minutes), RADIUS_MINUTEHAND);
}

// Handle representation for hours
void draw_hours(GContext *ctx, uint8_t hours, uint8_t minutes) {
  graphics_context_set_stroke_color(ctx, maybe_to_gray(s_palette->hours));
  graphics_context_set_stroke_width(ctx, LINE_THICKNESS);
  graphics_context_set_antialiased(ctx, true);
  draw_hand(ctx, angle_from_hours(hours * 60 + minutes), RADIUS_HOURHAND);
}

// Handle appointment circle
void draw_fullcircle(GContext *ctx) {
  graphics_context_set_stroke_color(ctx, maybe_to_gray(s_palette->circle));
  graphics_context_set_stroke_width(ctx, LINE_THICKNESS + 4);
  graphics_context_set_antialiased(ctx, true);
  
  graphics_draw_arc(ctx, s_screen, GOvalScaleModeFitCircle,
                    to_angle(0), to_angle(12 * 60));
}

void draw_appointments(GContext *ctx) {
  graphics_context_set_stroke_color(ctx, maybe_to_gray(s_palette->appointments));
  graphics_context_set_stroke_width(ctx, LINE_THICKNESS + 2);
  graphics_context_set_antialiased(ctx, true);
  
  for (int a = 0; a != 10; ++a) {
    if (appointment_start(a) == appointment_end(a)) {
      break;
    }
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Going to draw appt from %d to %d", (int)appointment_start(a), (int)appointment_end(a));
    graphics_draw_arc(ctx, s_screen, GOvalScaleModeFitCircle,
                      to_angle(appointment_start(a)), to_angle(appointment_end(a)));
  }
}

void draw_date(const char *text) {
  text_layer_set_text_color(s_date_text, maybe_to_gray(s_palette->date));
  text_layer_set_background_color(s_date_text, GColorClear);
  text_layer_set_text(s_date_text, text);
}