#include "../include/vector.h"

////////////////////////////////////////////////
// Vec2 Functions 
////////////////////////////////////////////////
vec2_t project(vec4_t point, float factor) {
  vec2_t projected = { 
    .x = (point.x*factor)/point.z, 
    .y = (point.y*factor)/point.z 
  };
  
  return projected;
}

vec2_t vec2_rotate_z(vec2_t v, float angle) {
  vec2_t rot = { 0, 0 };

  float cos_angle = cos(angle);
  float sin_angle = sin(angle);

  rot.x = v.x * cos_angle - v.y * sin_angle;
  rot.y = v.x * sin_angle + v.y * cos_angle;

  return rot;
}

float vec2_length(vec2_t v) {
  return sqrt(v.x*v.x + v.y*v.y);
}

vec2_t vec2_add(vec2_t a, vec2_t b) {
  vec2_t result = {
    .x = a.x + b.x,
    .y = a.y + b.y
  };

  return result;
}

vec2_t vec2_sub(vec2_t v0, vec2_t v1) {
  vec2_t result = {
    .x = v0.x - v1.x,
    .y = v0.y - v1.y
  };

  return result;
}

vec2_t vec2_mul(vec2_t v, float f) {
  vec2_t result = {
    .x = v.x * f,
    .y = v.y * f
  };

  return result;
}

vec2_t vec2_div(vec2_t v, float f) {
  vec2_t result = {
    .x = v.x / f,
    .y = v.y / f
  };

  return result;
}

float vec2_dot(vec2_t v0, vec2_t v1) {
  return (v0.x * v1.x) + (v0.y * v1.y);
}

float vec2_cross(vec2_t v0, vec2_t v1) {
  return (v0.x*v1.y) - (v0.y*v1.x);
}

/////////////////////////////////////////////////
// Vec3 Functions 
////////////////////////////////////////////////
vec3_t vec3_rotate_x(vec3_t v, float angle) {
  vec3_t rot = { 0, 0 };

  float cos_angle = cos(angle);
  float sin_angle = sin(angle);

  rot.x = v.x;
  rot.y = v.y * cos_angle - v.z * sin_angle;
  rot.z = v.y * sin_angle + v.z * cos_angle;

  return rot;
}

vec3_t vec3_rotate_y(vec3_t v, float angle) {
  vec3_t rot;

  float cos_angle = cos(angle);
  float sin_angle = sin(angle);

  rot.x = v.x * cos_angle - v.z * sin_angle;
  rot.y = v.y;
  rot.z = v.x * sin_angle + v.z * cos_angle;

  return rot;
}

vec3_t vec3_rotate_z(vec3_t v, float angle) {
  vec3_t rot = { 0, 0 };

  float cos_angle = cos(angle);
  float sin_angle = sin(angle);

  rot.x = v.x * cos_angle - v.y * sin_angle;
  rot.y = v.x * sin_angle + v.y * cos_angle;
  rot.z = v.z;

  return rot;
}

float vec3_length(vec3_t v) {
  return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

vec3_t vec3_add(vec3_t a, vec3_t b) {
  vec3_t result = {
    .x = a.x + b.x,
    .y = a.y + b.y,
    .z = a.z + b.z
  };

  return result;
}

vec3_t vec3_sub(vec3_t a, vec3_t b) {
  vec3_t result = {
    .x = a.x - b.x,
    .y = a.y - b.y,
    .z = a.z - b.z
  };

  return result;
}

vec3_t vec3_mul(vec3_t v, float f) {
  vec3_t result = {
    .x = v.x * f,
    .y = v.y * f,
    .z = v.z * f
  };

  return result;
}

vec3_t vec3_div(vec3_t v, float f) {
  vec3_t result = {
    .x = v.x / f,
    .y = v.y / f,
    .z = v.z / f
  };

  return result;
}

vec3_t vec3_cross(vec3_t a, vec3_t b) {
  vec3_t result = {
    .x = a.y*b.z - a.z*b.y,
    .y = a.x*b.z - a.z*b.x,
    .z = a.x*b.y - a.y*b.x
  };

  return result;
}

float vec3_dot(vec3_t v0, vec3_t v1) {
  return (v0.x*v1.x) + (v0.y*v1.y) + (v0.z*v1.z);
}

vec3_t vec3_from_vec4(vec4_t v) {
  vec3_t result = { v.x, v.y, v.z };
  return result;
}

/////////////////////////////////////////////////
// Vec4 Functions 
////////////////////////////////////////////////
vec4_t vec4_from_vec3(vec3_t v) {
  vec4_t result = { v.x, v.y, v.z, 1.0f };
  return result;
}
