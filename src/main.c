#include <pebble.h>
#include "app_state.h"
#include "app_data.h"

AppState appState;
AppData appData;

int main() {
  appstate_init(&appState, &appData);
  app_event_loop();
  appstate_deinit(&appState);
}
