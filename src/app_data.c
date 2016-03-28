#include "app_state.h"
#include "app_data_private.h"
#include "palette.h"
#include "storage.h"

void appdata_init(AppData *ad, AppState *as) {
  ad->appState = as;
  // Get palette info from localstore
  load_palette(&(ad->palette));
  
  // Load calendar data into appsync cache and start appsync
  appsync_setup_and_init(ad);
}

void appdata_deinit(AppData *ad) {
  // Flush calendar from appsync cache to localstore, then remove appsync
  save_calendar(&(ad->appSync));
  app_sync_deinit(&(ad->appSync));
  
  // Save palette to local store
  save_palette(&(ad->palette));
}
