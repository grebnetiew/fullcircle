#include "appointment.h"

#define CAL_VER_KEY 0
#define CAL_TIME_START_KEY(x) (2*x+2)
#define CAL_TIME_END_KEY(x) (2*x+3)

static AppSync s_sync;
static uint8_t s_sync_buffer[32];
extern Calendar s_cal;

static void sync_changed_handler(const uint32_t key, const Tuple *new_tuple, const Tuple *old_tuple, void *context) {
  // Update UI here
}

static void sync_error_handler(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
  // An error occured!
}

void calendar_init() {
  // Begin using AppMessage, needed by AppSync
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

  // Setup initial values
  Tuplet initial_values[] = {
    TupletInteger(CAL_VER_KEY, 1),
    TupletInteger(CAL_TIME_START_KEY(0), (int32_t)0),
    TupletInteger(CAL_TIME_END_KEY(0),   (int32_t)0),
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