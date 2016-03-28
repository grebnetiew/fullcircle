#include "app_state.h"
#include "app_data_private.h"
#include "palette.h"

// We update the internal calendar here
void appsync_cb_change(AppData *ad, const uint32_t key, const Tuple *new_tuple) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Message received with key %d\n", (int) key);
  if (key > 10 && key <= 16) {
    set(&(ad->palette), key, GColorFromHEX(new_tuple->value->uint32));
  }
  layer_mark_dirty(ad->appState->layer);
}

void appsync_cb_error(AppData *ad, DictionaryResult dict_error, AppMessageResult app_message_error) {
  // An error occured!
  // We are not really in a position to do anything about it.
  // Maybe in the future, we can display a Mysterious Error Icon?
  APP_LOG(APP_LOG_LEVEL_ERROR, "AppSync error %d, %d\n", dict_error, app_message_error);
}