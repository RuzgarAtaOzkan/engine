#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>

#include "../include/common.h"
#include "../include/display.h"
#include "../include/vec2.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define LOGS(s) (printf("LOGS: %s\n", s))
#define LOGD(d) (printf("LOGD: %d\n", d))
#define LOGF(f) (printf("LOGF: %f\n", f))

global_t global = {
  .time_dt = 0,
  .is_running = false
};

color_t colors[3] = {
  { .r = 0xFF, .g = 0x00, .b = 0x00 },
  { .r = 0x00, .g = 0xFF, .b = 0x00 },
  { .r = 0x00, .g = 0x00, .b = 0xFF }
};

vec2_t vertices[] = {
  { .x = 40, .y = 40 },
  { .x = 80, .y = 50 },
  { .x = 40, .y = 75 },
  { .x = 90, .y = 90 }
};

void input(void) {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        global.is_running = false;

        break;

      case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          global.is_running = false;
        }

        break; 
    }
  }
}

void fill_circle(uint8_t x, uint8_t y, int r, uint32_t color) {
  for (float i = 0.00f; i < M_PI*2; i = i + 0.01f) {
    if (i >= M_PI*2) {
      i = M_PI * 2;
    }

    uint8_t dx = x + r * cos(i);
    uint8_t dy = y - r * sin(i);
    
    draw_pixel(dx, dy, 0xFFFFFFFF);
  }
}

bool triangle_is_top_left(vec2_t *start, vec2_t *end) {
  vec2_t edge = { end->x - start->x, end->y - start->y };

  bool is_top_edge = edge.y == 0 && edge.x > 0;
  bool is_left_edge = edge.y < 0;

  return is_top_edge || is_left_edge;
}

float triangle_edge_cross(vec2_t * a, vec2_t *b, vec2_t *p) {
  // cross product of the second vector and current pixel relative to first vector arg
  vec2_t ab = { b->x - a->x, b->y - a->y };
  vec2_t ap = { p->x - a->x, p->y - a->y };
  return ab.x * ap.y - ab.y * ap.x;
}

void triangle_fill(vec2_t v0, vec2_t v1, vec2_t v2, uint32_t color) {
  // triangle boundaries
  float x_min = floor(MIN(MIN(v0.x, v1.x), v2.x));
  float y_min = floor(MIN(MIN(v0.y, v1.y), v2.y));
  float x_max = ceil(MAX(MAX(v0.x, v1.x), v2.x));
  float y_max = ceil(MAX(MAX(v0.y, v1.y), v2.y));

  float dt_w0_col = (v0.y - v1.y);
  float dt_w1_col = (v1.y - v2.y);
  float dt_w2_col = (v2.y - v0.y);

  float dt_w0_row = (v1.x - v0.x);
  float dt_w1_row = (v2.x - v1.x);
  float dt_w2_row = (v0.x - v2.x);

  //printf("col: %d %d %d\n", dt_w0_col, dt_w1_col, dt_w2_col);
  //printf("row: %d %d %d\n", dt_w0_row, dt_w1_row, dt_w2_row);

  int area = triangle_edge_cross(&v0, &v1, &v2);

  float bias0 = triangle_is_top_left(&v0, &v1) ? 0 : -0.001f;
  float bias1 = triangle_is_top_left(&v1, &v2) ? 0 : -0.001f;
  float bias2 = triangle_is_top_left(&v2, &v0) ? 0 : -0.001f;

  // cross product of the very first pixel
  vec2_t p0 = { x_min, y_min };
  float w0_row = triangle_edge_cross(&v0, &v1, &p0) + bias0;
  float w1_row = triangle_edge_cross(&v1, &v2, &p0) + bias1;
  float w2_row = triangle_edge_cross(&v2, &v0, &p0) + bias2;

  //printf("%d %d %d\n", w0_row, w1_row, w2_row);

  for (int y = y_min; y <= y_max; y++) {
    float w0 = w0_row;
    float w1 = w1_row;
    float w2 = w2_row;

    for (int x = x_min; x <= x_max; x++) {
      //printf("%f %f %f\n", w0, w1, w2);

      bool is_inside = w0 >= 0 && w1 >= 0 && w2 >= 0;

      if (is_inside) {
        float alpha = w0 / (float)area;
        float beta = w1 / (float)area;
        float gamma = w2 / (float)area;
        
        int a = 0xFF;
        int r = 0xFF * alpha;
        int g = 0xFF * beta;
        int b = 0xFF * gamma;

        uint32_t color_interp = 0x00000000;
        color_interp = (color_interp | a) << 8;
        color_interp = (color_interp | b) << 8;
        color_interp = (color_interp | g) << 8;
        color_interp = (color_interp | r);

        draw_pixel(x, y, color_interp);
      }

      w0 += dt_w0_col;
      w1 += dt_w1_col;
      w2 += dt_w2_col;
    }

    w0_row += dt_w0_row;
    w1_row += dt_w1_row;
    w2_row += dt_w2_row;

    //printf("%d %d %d\n", w0_row, w1_row, w2_row);
  }
}

void render() {
  framebuffer_clear(0xFF000000);

  vec2_t v0 = vertices[0];
  vec2_t v1 = vertices[1];
  vec2_t v2 = vertices[2];
  vec2_t v3 = vertices[3];
  
  vec2_t center = { 0.0f, 0.0f };
  float angle = SDL_GetTicks() / 1000.0f ;

  //v0 = vec2_rotate(vertices[0], center, angle);
  //v1 = vec2_rotate(vertices[1], center, angle);
  //v2 = vec2_rotate(vertices[2], center, angle);

  float x_min = floor(MIN(MIN(v1.x, v2.x), v3.x));
  float y_min = floor(MIN(MIN(v1.y, v2.y), v3.y));
  float x_max = ceil(MAX(MAX(v1.x, v2.x), v3.x));
  float y_max = ceil(MAX(MAX(v1.y, v2.y), v3.y));

  int width = x_max - x_min;
  int height = y_max - y_min;

  center.x = x_min + (float)(width/2);
  center.y = y_min + (float)(height/2);

  v3 = vec2_rotate(vertices[3], center, angle);
  v2 = vec2_rotate(vertices[2], center, angle);
  v1 = vec2_rotate(vertices[1], center, angle);

  //triangle_fill(v0, v1, v2, 0xFF00FF00);
  triangle_fill(v3, v2, v1, 0xFFA74DE3);

  printf("%f %f\n", SCREEN_WIDTH/2.0f * -1.f, SCREEN_HEIGHT/2.0f*-1.f);

  framebuffer_render();
}

float lerpf(float a, float b, float f) {
  float diff = fabs(a - b);

  if (a > b) {
    return diff * f + b;
  }

  return diff * f + a;
}

int lerp(int a, int b, float f) {
  float diff = abs(a - b);

  if (a > b) {
    return diff * f + b;
  }

  return diff * f + a;
}

int main(int argc, char **argv) {
  global.is_running = window_create();

  while (global.is_running) {
    framerate_fix2(&global.time_dt);
    input();
    render();
    //printf("delta time: %d\n", global.time_dt);
  }

  window_destroy();
  
  return EXIT_SUCCESS;
}
