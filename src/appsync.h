#include <pebble.h>

#define CALENDAR_LENGTH 10

inline uint32_t to_angle(uint32_t minutes) {
  return TRIG_MAX_ANGLE * minutes / (12 * 60);
}

inline uint32_t hexFromGColor(GColor c) {
  return 0x550000 * c.r + 0x5500 * c.g + 0x55 * c.b;
}

void appsync_init();
void appsync_deinit();

// Both these functions return time in minutes after 12
uint32_t appointment_start(uint8_t idx);
// The end of an appointment is always after the start, possibly > 12 * 60
uint32_t appointment_end(uint8_t idx);