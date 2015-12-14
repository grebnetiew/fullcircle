#include <pebble.h>

#define CALENDAR_LENGTH 10

typedef struct Appointment {
  uint32_t start; // in minutes after 12h
  uint32_t end;
} Appointment;

typedef Appointment Calendar[CALENDAR_LENGTH];

inline uint32_t to_angle(uint32_t minutes) {
  return TRIG_MAX_ANGLE * minutes / (12 * 60);
}

inline uint32_t hexFromGColor(GColor c) {
  return 0x550000 * c.r + 0x5500 * c.g + 0x55 * c.b;
}

void calendar_init();
void calendar_deinit();
