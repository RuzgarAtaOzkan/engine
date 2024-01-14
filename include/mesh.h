#ifndef MESH_H
#define MESH_H

#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "display.h"
#include "vector.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define LOGS(s) (printf("LOGS: %s\n", s))
#define LOGD(d) (printf("LOGD: %d\n", d))
#define LOGF(f) (printf("LOGF: %f\n", f))

typedef struct {
  int a;
  int b;
  int c;
  uint32_t color;
} face_t;

typedef struct {
  vec3_t rotation;
  size_t n_vertices;
  size_t n_faces;
  vec3_t *vertices;
  face_t *faces;
} mesh_t;

void fill_triangle(vec2_t v0, vec2_t v1, vec2_t v2, uint32_t color);

#endif /* MESH_H */
