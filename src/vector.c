#include "../include/vector.h"

vec2_t project(vec3_t point, float factor) {
  vec2_t projected = { 
    .x = (point.x*factor)/point.z, 
    .y = (point.y*factor)/point.z 
  };
  
  return projected;
}

vec2_t vec2_rotate_z(vec2_t v, vec2_t center, float angle) {
  vec2_t rot = { 0, 0 };

  float cos_angle = cos(angle);
  float sin_angle = sin(angle);

  //v.x = v.x - center.x;
  //v.y = v.y - center.y;

  rot.x = v.x * cos_angle - v.y * sin_angle;
  rot.y = v.x * sin_angle + v.y * cos_angle;

  //rot.x += center.x;
  //rot.y += center.y;

  return rot;
}

vec3_t vec3_rotate_x(vec3_t v, vec3_t center, float angle) {
  vec3_t rot = { 0, 0 };

  float cos_angle = cos(angle);
  float sin_angle = sin(angle);

  //v.x = v.x - center.x;
  //v.y = v.y - center.y;

  //v.z = v.z - center.z;

  rot.x = v.x;
  rot.y = v.y * cos_angle - v.z * sin_angle;
  rot.z = v.y * sin_angle + v.z * cos_angle;

  //rot.x += center.x;
  //rot.y += center.y;

  //rot.z = rot.z + center.z;

  return rot;
}

vec3_t vec3_rotate_y(vec3_t v, vec3_t center, float angle) {
  vec3_t rot;

  float cos_angle = cos(angle);
  float sin_angle = sin(angle);

  //v.x = v.x - center.x;
  //v.y = v.y - center.y;

  //v.z = v.z - center.z;

  rot.x = v.x * cos_angle - v.z * sin_angle;
  rot.y = v.y;
  rot.z = v.x * sin_angle + v.z * cos_angle;

  //rot.x += center.x;
  //rot.y += center.y;

  //rot.z = rot.z + center.z;

  return rot;
}

vec3_t vec3_rotate_z(vec3_t v, vec3_t center, float angle) {
  vec3_t rot = { 0, 0 };

  float cos_angle = cos(angle);
  float sin_angle = sin(angle);

  //v.x = v.x - center.x;
  //v.y = v.y - center.y;

  rot.x = v.x * cos_angle - v.y * sin_angle;
  rot.y = v.x * sin_angle + v.y * cos_angle;
  rot.z = v.z;

  //rot.x += center.x;
  //rot.y += center.y;

  return rot;
}
