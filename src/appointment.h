#include <pebble.h>

#define CALENDAR_LENGTH 10

typedef struct Appointment {
  int32_t start; // in minutes after 12h
  int32_t end;
} Appointment;

typedef Appointment Calendar[CALENDAR_LENGTH];

inline int32_t to_angle(int32_t minutes) {
  return TRIG_MAX_ANGLE * minutes / (12 * 60);
}
