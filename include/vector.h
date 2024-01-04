#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>

typedef struct {
  float x;
  float y;
} vec2_t;

typedef struct {
  float x;
  float y;
  float z;
} vec3_t;

typedef struct {
  vec3_t pos;
  vec3_t rot;
  float fov;
} cam_t;

vec2_t project(vec3_t point, float factor);
vec2_t vec2_rotate_z(vec2_t v, vec2_t center, float angle);

vec3_t vec3_rotate_x(vec3_t v, vec3_t center, float angle);
vec3_t vec3_rotate_y(vec3_t v, vec3_t center, float angle);
vec3_t vec3_rotate_z(vec3_t v, vec3_t center, float angle);

#endif /* VECTOR_H */
