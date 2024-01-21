#include "../include/matrix.h"

#define LOGS(s) (printf("LOGS: %s\n", s))
#define LOGD(d) (printf("LOGD: %d\n", d))
#define LOGF(f) (printf("LOGF: %f\n", f))

mat4_t matrix_proj = {
  .m = {
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 }
  }
};

mat4_t mat4_identity(void) {
  // | 1 0 0 0 |
  // | 0 1 0 0 |
  // | 0 0 1 0 |
  // | 0 0 0 1 |
  mat4_t m = {
    .m = {
      { 1, 0, 0, 0 },
      { 0, 1, 0, 0 },
      { 0, 0, 1, 0 },
      { 0, 0, 0, 1 }
    }
  };

  return m;
}

mat4_t mat4_scale(float sx, float sy, float sz) {
  mat4_t m = mat4_identity();

  m.m[0][0] = sx;
  m.m[1][1] = sy;
  m.m[2][2] = sz;

  return m;
}

mat4_t mat4_rotate_x(float angle) {
  float c = cos(angle);
  float s = sin(angle);

  mat4_t m = mat4_identity();

  m.m[1][1] = c;

  m.m[1][2] = -s;

  m.m[2][1] = s;

  m.m[2][2] = c;

  return m;
}

mat4_t mat4_rotate_y(float angle) {
  float c = cos(angle);
  float s = sin(angle);

  mat4_t m = mat4_identity();

  m.m[0][0] = c;
  m.m[0][2] = -s;
  
  m.m[2][0] = s;
  m.m[2][2] = c;

  return m;
}

mat4_t mat4_rotate_z(float angle) {
  float c = cos(angle);
  float s = sin(angle);

  mat4_t m = mat4_identity();

  m.m[0][0] = c;
  m.m[0][1] = -s;

  m.m[1][0] = s;
  m.m[1][1] = c;

  return m;
}

mat4_t mat4_translate(float tx, float ty, float tz) {
  mat4_t m = mat4_identity();

  m.m[0][3] = tx;
  m.m[1][3] = ty;
  m.m[2][3] = tz;

  return m;
}

vec4_t mat4_mul_vec4(mat4_t m, vec4_t v) {
  vec4_t result;

  result.x = m.m[0][0]*v.x + m.m[0][1]*v.y + m.m[0][2]*v.z + m.m[0][3]*v.w;
  result.y = m.m[1][0]*v.x + m.m[1][1]*v.y + m.m[1][2]*v.z + m.m[1][3]*v.w;
  result.z = m.m[2][0]*v.x + m.m[2][1]*v.y + m.m[2][2]*v.z + m.m[2][3]*v.w;
  result.w = m.m[3][0]*v.x + m.m[3][1]*v.y + m.m[3][2]*v.z + m.m[3][3]*v.w;

  return result;
}

mat4_t mat4_mul_mat4(mat4_t a, mat4_t b) {
  mat4_t result = mat4_identity();

  for (uint8_t i = 0; i < 4; i++) {
    for (uint8_t j = 0; j < 4; j++) {
      result.m[i][j] = 
      a.m[i][0]*b.m[0][j] + 
      a.m[i][1]*b.m[1][j] + 
      a.m[i][2]*b.m[2][j] + 
      a.m[i][3]*b.m[3][j];
    }
  }

  return result;
}

mat4_t mat4_make_perspective(float fov, float aspect, float znear, float zfar) {

  mat4_t m = {
    .m = {
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 }
    }
  };

  m.m[0][0] = aspect * (1.0f / tan(fov / 2.0f)); // 0.5625 when fov is PI/2. 0.5625 is window_height/window_width

  m.m[1][1] = (1.0f / tan(fov / 2.0f)); // 1.000 when fov is PI/2
  
  m.m[2][2] = zfar / (zfar - znear);
  m.m[2][3] = (-zfar *znear) / (zfar - znear);

  m.m[3][2] = 1.0f;

  /*  
    { 0.5625,     0,        0,      0 },
    {      0, 1.000,        0,      0 },
    {      0,     0,   1.0010, -0.100 },
    {      0,     0,      1.0,      0 }
  */

  return m;
}

vec4_t mat4_mul_vec4_project(mat4_t mat_proj, vec4_t v) {
  vec4_t result = mat4_mul_vec4(mat_proj, v);

  if (result.w == 0.0) {
    return result;
  }

  result.x /= result.w;
  result.y /= result.w;
  result.z /= result.w;

  return result;
}
