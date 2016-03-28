#include "app_state_private.h"
#include "drawing.h"

void appstate_window_init(AppState *as) {
  as->layer = layer_create(layer_get_bounds(window_get_root_layer(as->window)));
  as->screen = grect_inset(layer_get_bounds(as->layer), GEdgeInsets(SCREEN_BORDER));
  layer_add_child(window_get_root_layer(as->window), as->layer);
  layer_set_update_proc(as->layer, update_display);
  
  GPoint center = grect_center_point(&(as->screen));
  // Make three possible locations for the date
  as->date_positions[0] = GRect(center.x + 15, center.y - 9,  60, 18);
  as->date_positions[1] = GRect(center.x - 30, center.y + 15, 60, 18);
  as->date_positions[2] = GRect(center.x - 30, center.y - 33, 60, 18);
  
  as->date_text = text_layer_create(GRect(center.x + 15, center.y - 9, 60, 18));
  layer_add_child(as->layer, text_layer_get_layer(as->date_text));
}

void appstate_window_deinit(AppState *as) {
  text_layer_destroy(as->date_text);
  layer_destroy(as->layer);
}
