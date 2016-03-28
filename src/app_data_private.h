#include "app_data.h"
#include "globals.h"

// Declarations of private member functions

// AppSync requires callbacks for changes and errors
void appsync_cb_change(AppData *ad, const uint32_t key, const Tuple *new_tuple);
void appsync_cb_error(AppData *ad, DictionaryResult dict_error, AppMessageResult app_message_error);

// Load calendar data into appsync cache and start appsync
void appsync_setup_and_init(AppData *ad);
