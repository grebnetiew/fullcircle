#include "appointment.h"

#define CAL_CURRENT_VER 1
#define CAL_BUFFER_SIZE 128
// That's 1 + 11 tuples x (7+4) bytes per tuple, and 6 extra for.. alignment
#define CAL_VER_KEY 0
#define CAL_CURRENT_VER 1
static inline uint8_t cal_start_key_from_idx(uint8_t idx) {
  return 2 * idx + 1;
}
static inline uint8_t cal_end_key_from_idx(uint8_t idx) {
  return 2 * idx + 2;
}
static inline uint8_t cal_start_idx_from_key(uint8_t key) {
  return (key - 1) / 2;
}
static inline uint8_t cal_end_idx_from_key(uint8_t key) {
  return (key - 2) / 2;
}
static inline bool cal_is_start_key(uint8_t key) {
  return key % 2 == 1;
}

static AppSync s_sync;
static uint8_t s_sync_buffer[CAL_BUFFER_SIZE];

extern Calendar s_cal;
extern Layer *s_layer;

// We update the internal calendar here if the 
static void sync_changed_handler(const uint32_t key, const Tuple *new_tuple, const Tuple *old_tuple, void *context) {
  if (key == 0) { // Though this really shouldn't happen
    return;
  }
  if (cal_is_start_key(key)) {
    s_cal[cal_start_idx_from_key(key)].start = new_tuple->value->uint32;
  } else {
    s_cal[cal_end_idx_from_key(key)].end = new_tuple->value->uint32;
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
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

  // Setup initial values
  Tuplet initial_values[] = {
    TupletInteger(CAL_VER_KEY, (int32_t) CAL_CURRENT_VER),
    TupletInteger(cal_start_key_from_idx(0), (int32_t)0),
    TupletInteger(cal_end_key_from_idx(0),   (int32_t)0),
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