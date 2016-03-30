#pragma once
#include <pebble.h>
#include "globals.h"

void update_display(Layer *layer, GContext *ctx);

void draw_minutes(GContext *ctx, uint8_t minutes);
void draw_hours(GContext *ctx, uint8_t hours, uint8_t minutes);

void draw_appointments(GContext *ctx);
void draw_fullcircle(GContext *ctx);
void draw_piechart_appointments(GContext *ctx);

void draw_date(const char *text, uint8_t hours, uint8_t minutes);

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