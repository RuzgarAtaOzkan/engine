#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
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
  float x;
  float y;
  float z;
  float w;
} vec4_t;

typedef struct {
  vec3_t translation;
  vec3_t rotation;
  float fov;
} cam_t;

vec2_t project(vec4_t point, float factor);
vec2_t vec2_rotate_z(vec2_t v, float angle);

float vec2_length(vec2_t v);

vec2_t vec2_add(vec2_t a, vec2_t b);
vec2_t vec2_sub(vec2_t a, vec2_t b);
vec2_t vec2_mul(vec2_t v, float f);
vec2_t vec2_div(vec2_t v, float f);

float vec2_dot(vec2_t v0, vec2_t v1);
float vec2_cross(vec2_t v0, vec2_t v1);

vec3_t vec3_rotate_x(vec3_t v, float angle);
vec3_t vec3_rotate_y(vec3_t v, float angle);
vec3_t vec3_rotate_z(vec3_t v, float angle);

float vec3_length(vec3_t v);

vec3_t vec3_add(vec3_t a, vec3_t b);
vec3_t vec3_sub(vec3_t a, vec3_t b);
vec3_t vec3_mul(vec3_t v, float f);
vec3_t vec3_div(vec3_t v, float f);

float vec3_dot(vec3_t a, vec3_t b);
vec3_t vec3_cross(vec3_t a, vec3_t b);

vec3_t vec3_from_vec4(vec4_t v);

// VEC 4
vec4_t vec4_from_vec3(vec3_t v);

#endif /* VECTOR_H */
