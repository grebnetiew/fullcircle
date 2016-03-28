#include <pebble.h>
#include "drawing.h"
#include "app_state.h"
#include "app_data.h"

extern AppState appState;
extern AppData appData;

void update_display(Layer *layer, GContext *ctx) {
  window_set_background_color(appState.window, appData.palette.background);
  
  if (gcolor_equal(appData.palette.circle, appData.palette.appointments)) {
    // A nonsense setting used to encode "draw me a pie chart"
    draw_piechart_appointments(ctx);
  } else {
    draw_fullcircle(ctx);
    draw_appointments(ctx);
  }
  
  draw_date(appState.date, appState.hour % 12, appState.minute);
  
  // Draw clock
  draw_minutes(ctx, appState.minute);
  draw_hours(ctx, appState.hour % 12, appState.minute);
}

void draw_hand(GContext *ctx, uint32_t angle, uint8_t length) {
  // Calculate the endpoints of the lines
  GPoint center = grect_center_point(&appState.screen);
  GPoint hand   = GpointFromPolar(center, angle, length);
  GPoint wrist  = GpointFromPolar(center, angle, length / 9);
  
  // Draw hands
  graphics_draw_line(ctx, wrist, hand);
  graphics_draw_circle(ctx, center, length / 10);
}

// Handle representation for minutes
void draw_minutes(GContext *ctx, uint8_t minutes) {
  graphics_context_set_stroke_color(ctx, maybe_to_gray(appData.palette.minutes));
  graphics_context_set_stroke_width(ctx, LINE_THICKNESS + 1);
  graphics_context_set_antialiased(ctx, true);
  
  draw_hand(ctx, angle_from_minutes(minutes), RADIUS_MINUTEHAND);
}

// Handle representation for hours
void draw_hours(GContext *ctx, uint8_t hours, uint8_t minutes) {
  graphics_context_set_stroke_color(ctx, maybe_to_gray(appData.palette.hours));
  graphics_context_set_stroke_width(ctx, LINE_THICKNESS);
  graphics_context_set_antialiased(ctx, true);
  draw_hand(ctx, angle_from_hours(hours * 60 + minutes), RADIUS_HOURHAND);
}

// Handle appointment circle
void draw_fullcircle(GContext *ctx) {
  graphics_context_set_stroke_color(ctx, maybe_to_gray(appData.palette.circle));
  graphics_context_set_stroke_width(ctx, LINE_THICKNESS + 4);
  graphics_context_set_antialiased(ctx, true);
  
  graphics_draw_arc(ctx, appState.screen, GOvalScaleModeFitCircle,
                    angle_from_hours(0), angle_from_hours(12 * 60));
}

void draw_appointments(GContext *ctx) {
  graphics_context_set_stroke_color(ctx, maybe_to_gray(appData.palette.appointments));
  graphics_context_set_stroke_width(ctx, LINE_THICKNESS + 2);
  graphics_context_set_antialiased(ctx, true);
  
  for (int a = 0; a != 10; ++a) {
    if (appointment_start(&appData, a) == appointment_end(&appData, a)) {
      break;
    }
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Going to draw appt from %d to %d", (int)appointment_start(&appData, a), (int)appointment_end(&appData, a));
    graphics_draw_arc(ctx, appState.screen, GOvalScaleModeFitCircle,
                      angle_from_hours(appointment_start(&appData, a)), angle_from_hours(appointment_end(&appData, a)));
  }
}

void draw_piechart_appointments(GContext *ctx){
  graphics_context_set_fill_color(ctx, maybe_to_gray(appData.palette.appointments));
  graphics_context_set_antialiased(ctx, true);
  GRect canvas = (GRect){
    .origin = GPoint(appState.screen.origin.x - 50, appState.screen.origin.y - 50),
    .size   = GSize(appState.screen.size.w + 100, appState.screen.size.h + 100)
  };
  
  for (int a = 0; a != 10; ++a) {
    if (appointment_start(&appData, a) == appointment_end(&appData, a)) {
      break;
    }
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Going to draw appt pie from %d to %d", (int)appointment_start(&appData, a), (int)appointment_end(&appData, a));
    graphics_fill_radial(ctx, canvas, GOvalScaleModeFillCircle, appState.screen.size.w,
                      angle_from_hours(appointment_start(&appData, a)), angle_from_hours(appointment_end(&appData, a)));
  }
}

void draw_date(const char *text, uint8_t hours, uint8_t minutes) {
  text_layer_set_text_color(appState.date_text, maybe_to_gray(appData.palette.date));
  text_layer_set_background_color(appState.date_text, GColorClear);
  text_layer_set_text(appState.date_text, text);
  
  // Put the layer in a position not obscured by hands
  if ((hours < 2 || hours >= 4) && (minutes < 10 || minutes >= 20)) {
    // Standard place, to the right
    layer_set_frame(text_layer_get_layer(appState.date_text), appState.date_positions[0]);
    text_layer_set_text_alignment(appState.date_text, GTextAlignmentLeft);
  } else if ((hours < 4 || hours >= 9) && (minutes < 20 || minutes >= 45)) {
    // Alternative, below the center
    layer_set_frame(text_layer_get_layer(appState.date_text), appState.date_positions[1]);
    text_layer_set_text_alignment(appState.date_text, GTextAlignmentCenter);
  } else {
    // Alternative, above the center
    layer_set_frame(text_layer_get_layer(appState.date_text), appState.date_positions[2]);
    text_layer_set_text_alignment(appState.date_text, GTextAlignmentCenter);
  }
  layer_mark_dirty(text_layer_get_layer(appState.date_text));
}