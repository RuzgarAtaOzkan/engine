#include "../include/vector.h"
#include "../include/display.h"
#include "../include/utils.h"

void my_draw_line(float x_start, float y_start, float x_end, float y_end, uint32_t color) {
  if (x_start < 0 || y_start < 0 || x_end >= window_width || y_end >= window_height) {
    return;
  }

  // pixel gaps bug TODO

  float dx = x_end - x_start;
  float dy = y_end - y_start;
  float ratio = .0f;

  if (abs(dx) > abs(dy)) {
    if (dy == 0.0f) {
      dy = 1.f;
    }

    ratio = fabs(dy/dx);
    for (int i = 0; i < (int)fabs(dx); i++) {
      uint32_t x = round(x_start + 1/dx * i * fabs(dx));
      uint32_t y = round(y_start + 1/dy * i * fabs(dy) * ratio);
      draw_pixel(x, y, color);
    }

    return;
  }

  if (dx == 0.0f) {
    dx = 1.f;
  }

  ratio = fabs(dx/dy);
  for (int i = 0; i < (int)fabs(dy); i++) {
    uint32_t x = round(x_start + 1/dx * i * fabs(dx) * ratio);
    uint32_t y = round(y_start + 1/dy * i * fabs(dy));
    draw_pixel(x, y, color);
  }
}

void my_draw_line2(float x0, float y0, float x1, float y1, uint32_t color) {

  float dx = abs(x1-x0);
  float dy = abs(y1-y0);

  float r = dy/dx;

  float y = y0;

  for (int x = x0; x <= x1; x++) {

    for (int j = 0; j < r; j++) {
      draw_pixel(x, y+j, color);
    }

    y = y + r;
  }
}

int fac(int n) {
  int r = 1;
  for (int i = 1; i <= n; i++) {
    r = r * i;
  }

  return r;
}

float cos_to_sine(float c) {
  float x = .358f;
  float y = sqrt(1.00f - (x*x));
  return y;
}

void *f_obj_load(FILE *file, char c, size_t *length) {
  if (file == NULL) {
    return NULL;
  }

  uint8_t i_linecc = 0; // line char check index

  if (c != 'v' && c != 'f') {
    i_linecc = 1;
  }

  uint8_t n_line = 64;
  uint8_t n_num = 32;

  char line[n_line]; // current line buffer
  char num[n_num]; // number buffer

  for (size_t i = 0; i < n_line; i++) {
    line[i] = '\0';
  }

  for (size_t j = 0; j < n_num; j++) {
    num[j] = '\0';
  }

  // length of the array that will be allocated
  size_t n_lines = 0;
  while (fgets(line, sizeof(line) / sizeof(line[0]), file)) {
    // lines starts with -> v
    if (line[i_linecc] == c && line[i_linecc+1] == ' ') {
      n_lines++;
    }
  }

  *length = n_lines;
  size_t i_output = 0;
  void *output = NULL;
  
  switch (c) {
    case 'v':
    case 'n':
    case 't':
      output = malloc(sizeof(vec3_t)*n_lines);
      break;
    case 'f':
      output = malloc(sizeof(face_t)*n_lines);
      break;
  }

  fseek(file, 0, SEEK_SET);

  while (fgets(line, sizeof(line) / sizeof(line[0]), file)) {
    // lines starts with -> v
    if (line[i_linecc] != c || line[i_linecc+1] != ' ') {
      // go to next line if line doesnt start with given character and space
      // e.g. v 0.123 -0.322 1.3223
      continue;
    }

    uint8_t i_num = 0;
    uint8_t i_xyz = 0;
    bool num_read = false;

    for (size_t i = 0; i < n_line; i++) {
      if (line[i] == '\n') {
        // z value
        //printf("%f\n", atof(num));

        if (c != 'f') {
          ((vec3_t*)output)[i_output].z = atof(num);
        } else {
          ((face_t*)output)[i_output].c = atoi(num);
        }

        // clear num char array
        for (size_t j = 0; j < n_num; j++) {
          num[j] = '\0';
        }

        break;
      }
      
      if (line[i] == ' ') {
        if (i_xyz == 1) {
          // x value
          //printf("%f\n", atof(num));

          if (c != 'f') {
            ((vec3_t*)output)[i_output].x = atof(num);
          } else {
            ((face_t*)output)[i_output].a = atoi(num);
          }
        }

        if (i_xyz == 2) {
          // y value
          //printf("%f\n", atof(num));
          if (c != 'f') {
            ((vec3_t*)output)[i_output].y = atof(num);
          } else {
            ((face_t*)output)[i_output].b = atoi(num);
          }
        }

        // clear num char array
        for (size_t j = 0; j < n_num; j++) {
          num[j] = '\0';
        }

        i_num = 0;
        i_xyz++;
        num_read = true;

        continue;
      }

      if (line[i] == '/') {
        num_read = false;
        continue;
      }

      if (num_read) {
        num[i_num] = line[i];
        i_num++;
      }
    }

    // clear line char array
    for (size_t i = 0; i < n_line; i++) {
      line[i] = '\0';
    }

    i_output++;
  }

  fseek(file, 0, SEEK_SET);

  return output;
}
