#ifndef COMMON_H
#define COMMON_H

typedef struct {
  int time_dt;
  bool is_running;
  float rotate_v;
} global_t;

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} color_t;

#endif
