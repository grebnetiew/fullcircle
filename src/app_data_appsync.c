#include "app_data_private.h"
#include "storage.h"

// Now here comes the dirty ass hack
// Can't pass a method pointer as callback to appsync
// So how does Appsync know where to put its synced stuff?
static AppData *appDataForAppSync;
// And now we use that in the callbacks to call a proper method
static void sync_changed_handler(const uint32_t key, const Tuple *new_tuple, const Tuple *old_tuple, void *context) {
  appsync_cb_change(appDataForAppSync, key, new_tuple);
}
static void sync_error_handler(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
  appsync_cb_error(appDataForAppSync, dict_error, app_message_error);
}

void appsync_setup_and_init(AppData *ad) {
  // Begin using AppMessage, needed by AppSync
  app_message_open(2 * CAL_BUFFER_SIZE, 2 * CAL_BUFFER_SIZE);

  // Setup initial values
  uint8_t initval[CAL_DATA_LENGTH];
  for (int i = 0; i != CAL_DATA_LENGTH; ++i) {
    initval[i] = 0;
  }
  load_calendar(initval, CAL_DATA_LENGTH);
  
  Tuplet initial_values[] = {
    TupletInteger(CAL_VER_KEY, (int32_t) CAL_CURRENT_VER),
    TupletBytes  (CAL_DATA_KEY, initval, CAL_DATA_LENGTH),
    TupletInteger(KEY_COL_HOUR + 10,        hexFromGColor(ad->palette.hours)),
    TupletInteger(KEY_COL_MINUTE + 10,      hexFromGColor(ad->palette.minutes)),
    TupletInteger(KEY_COL_APPOINTMENT + 10, hexFromGColor(ad->palette.appointments)),
    TupletInteger(KEY_COL_CIRCLE + 10,      hexFromGColor(ad->palette.circle)),
    TupletInteger(KEY_COL_BACKGROUND + 10,  hexFromGColor(ad->palette.background)),
    TupletInteger(KEY_COL_DATE + 10,        hexFromGColor(ad->palette.date))
  };

  // Begin using AppSync
  appDataForAppSync = ad;
  app_sync_init(&ad->appSync, ad->sync_buffer, sizeof(ad->sync_buffer),
                initial_values, ARRAY_LENGTH(initial_values),
                sync_changed_handler, sync_error_handler,
                NULL /* context */);
}
