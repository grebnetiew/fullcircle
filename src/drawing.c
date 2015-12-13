#include <pebble.h>
#include "ui.h"

extern GRect s_screen;

inline uint32_t angle_from_minutes(uint8_t min) {
  return TRIG_MAX_ANGLE * min / 60;
}

inline uint32_t angle_from_hours(uint32_t min) {
  return TRIG_MAX_ANGLE * min / (12 * 60);
}

void draw_hand(GContext *ctx, uint32_t angle, uint8_t length) {
  // Calculate the endpoints of the lines
  GPoint center = grect_center_point(&s_screen);;
  GPoint hand = GPoint(
    center.x + length * sin_lookup(angle) / TRIG_MAX_RATIO,
    center.y - length * cos_lookup(angle) / TRIG_MAX_RATIO);
  GPoint wrist = GPoint(
    center.x + length * sin_lookup(angle) / (TRIG_MAX_RATIO * 9),
    center.y - length * cos_lookup(angle) / (TRIG_MAX_RATIO * 9));
  
  // Draw hands
  graphics_context_set_antialiased(ctx, true);
  graphics_draw_line(ctx, wrist, hand);
  graphics_draw_circle(ctx, center, length / 10);
}

// Handle representation for minutes
void draw_minutes(GContext *ctx, uint8_t minutes) {
  graphics_context_set_stroke_width(ctx, LINE_THICKNESS + 1);
  draw_hand(ctx, angle_from_minutes(minutes), RADIUS_MINUTEHAND);
}

// Handle representation for hours
void draw_hours(GContext *ctx, uint8_t hours, uint8_t minutes) {
  graphics_context_set_stroke_width(ctx, LINE_THICKNESS);
  draw_hand(ctx, angle_from_hours(hours * 60 + minutes), RADIUS_HOURHAND);
}

// Handle appointment circle
void draw_fullcircle(GContext *ctx) {
  graphics_context_set_stroke_width(ctx, 1);
  graphics_draw_arc(ctx, s_screen, GOvalScaleModeFitCircle,
                    to_angle(0), to_angle(12 * 60));
}

void draw_appointments(GContext *ctx, Appointment *cal) {
  graphics_context_set_stroke_width(ctx, LINE_THICKNESS + 3);
  for (Appointment *a = cal, *end = cal + 10; a != end; ++a) {
    if (a->start == a->end) {
      break;
    }
    graphics_draw_arc(ctx, s_screen, GOvalScaleModeFitCircle,
                      to_angle(a->start), 
                      to_angle(a->end < a->start ? a->end + 12 * 60 : a->end));
  }
}