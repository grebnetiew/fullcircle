#include "appsync.h"
#include "storage.h"

#define CAL_CURRENT_VER 1
#define CAL_BUFFER_SIZE 128
/* Size calc: 114
   Dictionary hdr 1 (1)
   Version tuple: 7 header + 4 data (11)
   Storage: 40 bytes + 7 hdr (47)
   Colors: 5 tuples, each 7 hdr and 4 data. (55)
*/
#define DATA_LENGTH 40

enum DataKeys {
  CAL_VER_KEY = 0,
  CAL_DATA_KEY = 1,
};

static AppSync s_sync;
static uint8_t s_sync_buffer[CAL_BUFFER_SIZE];

extern Palette *s_palette;
extern Layer *s_layer;

// We update the internal calendar here
static void sync_changed_handler(const uint32_t key, const Tuple *new_tuple, const Tuple *old_tuple, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Message received with key %d\n", (int) key);
  if (key > 10 && key <= 16) {
    set_palette(key, GColorFromHEX(new_tuple->value->uint32));
  }
  layer_mark_dirty(s_layer);
}

static void sync_error_handler(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
  // An error occured!
  // We are not really in a position to do anything about it.
  // Maybe in the future, we can display a Mysterious Error Icon?
  APP_LOG(APP_LOG_LEVEL_ERROR, "AppSync error %d, %d\n", dict_error, app_message_error);
}

void appsync_init() {
  // Begin using AppMessage, needed by AppSync
  app_message_open(2 * CAL_BUFFER_SIZE, 2 * CAL_BUFFER_SIZE);

  // Setup initial values
  uint8_t zeros[DATA_LENGTH];
  for (int i = 0; i != DATA_LENGTH; ++i) {
    zeros[i] = 0;
  }
  
  Tuplet initial_values[] = {
    TupletInteger(CAL_VER_KEY, (int32_t) CAL_CURRENT_VER),
    TupletBytes  (CAL_DATA_KEY, zeros, DATA_LENGTH),
    TupletInteger(KEY_COL_HOUR + 10,        hexFromGColor(s_palette->hours)),
    TupletInteger(KEY_COL_MINUTE + 10,      hexFromGColor(s_palette->minutes)),
    TupletInteger(KEY_COL_APPOINTMENT + 10, hexFromGColor(s_palette->appointments)),
    TupletInteger(KEY_COL_CIRCLE + 10,      hexFromGColor(s_palette->circle)),
    TupletInteger(KEY_COL_BACKGROUND + 10,  hexFromGColor(s_palette->background)),
    TupletInteger(KEY_COL_DATE + 10,        hexFromGColor(s_palette->date))
  };

  // Begin using AppSync
  app_sync_init(&s_sync, s_sync_buffer, sizeof(s_sync_buffer),
                initial_values, ARRAY_LENGTH(initial_values),
                sync_changed_handler, sync_error_handler,
                NULL /* context */);
}

void appsync_deinit() {
  app_sync_deinit(&s_sync);
}

uint32_t appointment_start(uint8_t idx) {
  const Tuple *t = app_sync_get(&s_sync, CAL_DATA_KEY);
  return t->value->data[4*idx] * 60 + t->value->data[4*idx + 1];
}

uint32_t appointment_end(uint8_t idx) {
  const Tuple *t = app_sync_get(&s_sync, CAL_DATA_KEY);
  uint32_t end = t->value->data[4*idx + 2] * 60 + t->value->data[4*idx + 3];
  return end < appointment_start(idx) ? end + 12 * 60 : end;
}
