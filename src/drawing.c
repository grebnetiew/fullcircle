#include <pebble.h>
#include "ui.h"

extern GRect s_screen;

int32_t angle_from_minutes(uint8_t min) {
  return TRIG_MAX_ANGLE * min / 60;
}

int32_t angle_from_hours(int32_t min) {
  return TRIG_MAX_ANGLE * min / (12 * 60);
}

void draw_hand(GContext *ctx, int32_t angle, uint8_t length) {
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
  draw_hand(ctx, angle_from_minutes(minutes), RADIUS_MINUTEHAND);
}

// Handle representation for hours
void draw_hours(GContext *ctx, uint8_t hours, uint8_t minutes) {
  draw_hand(ctx, angle_from_hours(hours * 60 + minutes), RADIUS_HOURHAND);
}

// Durr
void draw_appointments(GContext *ctx, Appointment *cal) {
  for (Appointment *a = cal, *end = cal + 10; a != end; ++a) {
    if (a->start == a->end) {
      break;
    }
    graphics_draw_arc(ctx, s_screen, GOvalScaleModeFitCircle,
                      to_angle(a->start), 
                      to_angle(a->end < a->start ? a->end + 12 * 60 : a->end));
  }
}