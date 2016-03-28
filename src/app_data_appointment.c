#include "app_data_private.h"

uint32_t appointment_start(AppData *ad, uint8_t idx) {
  const Tuple *t = app_sync_get(&(ad->appSync), CAL_DATA_KEY);
  return t->value->data[4*idx] * 60 + t->value->data[4*idx + 1];
}

uint32_t appointment_end(AppData *ad, uint8_t idx) {
  const Tuple *t = app_sync_get(&(ad->appSync), CAL_DATA_KEY);
  uint32_t end = t->value->data[4*idx + 2] * 60 + t->value->data[4*idx + 3];
  return end < appointment_start(ad, idx) ? end + 12 * 60 : end;
}
