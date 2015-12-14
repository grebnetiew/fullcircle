#include "appointment.h"
#include "storage.h"

#define CAL_CURRENT_VER 1
#define CAL_BUFFER_SIZE 64
// Thats 1 + 11 (version tuple) + 4 (bytes) x 10 (appt) + 7 (tuple hdr) = 59. And some slack.
#define DATA_LENGTH 40

enum DataKeys {
  CAL_VER_KEY = 0,
  CAL_DATA_KEY = 1,
};

static AppSync s_sync;
static uint8_t s_sync_buffer[CAL_BUFFER_SIZE];

extern Calendar s_cal;
extern Layer *s_layer;

// We update the internal calendar here
static void sync_changed_handler(const uint32_t key, const Tuple *new_tuple, const Tuple *old_tuple, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Message received with key %d\n", (int) key);
  if (key > 10 && key <= 15) {
    set_palette(key, GColorFromHEX(new_tuple->value->uint32));
    return;
  }
  if (key != 1 || new_tuple->length != DATA_LENGTH) { 
    return;
  }
  // fill calendar with new data
  for (int i = 0; i != 10; ++i) {
    s_cal[i].start = new_tuple->value->data[4*i]     * 60 + new_tuple->value->data[4*i + 1];
    s_cal[i].end   = new_tuple->value->data[4*i + 2] * 60 + new_tuple->value->data[4*i + 3];
  }
  layer_mark_dirty(s_layer);
}

static void sync_error_handler(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
  // An error occured!
  // We are not really in a position to do anything about it.
  // Maybe in the future, we can display a Mysterious Error Icon?
}

void calendar_init() {
  // Begin using AppMessage, needed by AppSync
  app_message_open(app_message_inbox_size_maximum() / 2, app_message_outbox_size_maximum() / 2);

  // Setup initial values
  uint8_t zeros[DATA_LENGTH];
  for (int i = 0; i != DATA_LENGTH; ++i) {
    zeros[i] = 0;
  }
  
  Tuplet initial_values[] = {
    TupletInteger(CAL_VER_KEY, (int32_t) CAL_CURRENT_VER),
    TupletBytes  (CAL_DATA_KEY, zeros, DATA_LENGTH),
  };

  // Begin using AppSync
  app_sync_init(&s_sync, s_sync_buffer, sizeof(s_sync_buffer),
                initial_values, ARRAY_LENGTH(initial_values),
                sync_changed_handler, sync_error_handler,
                NULL /* context */);
}

void calendar_deinit() {
  app_sync_deinit(&s_sync);
}