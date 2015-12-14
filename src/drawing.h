#include <pebble.h>
#include "appsync.h"

#define SCREEN_BORDER 10
#define RADIUS_HOURHAND 45
#define RADIUS_MINUTEHAND 60
#define LINE_THICKNESS 3

void draw_minutes(GContext *ctx, uint8_t minutes);
void draw_hours(GContext *ctx, uint8_t hours, uint8_t minutes);

void draw_appointments(GContext *ctx);
void draw_fullcircle(GContext *ctx);