#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>

#define FPS 60
#define MS_PER_FRAME (1000 / FPS)

enum cull_method {
  CULL_NONE,
  CULL_BACKFACE
};

enum render_method {
  RENDER_WIRE,
  RENDER_WIRE_VERTEX,
  RENDER_FILL_TRIANGLE,
  RENDER_FILL_TRIANGLE_WIRE
};

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Texture *color_buffer_texture;

extern uint32_t *color_buffer;

extern int window_width;
extern int window_height;

bool window_init(void);
void window_destroy(void);

void color_buffer_render(void);
void color_buffer_clear(uint32_t color);

void framerate_fix(int *dt);

void draw_pixel(int x, int y, uint32_t color);
void draw_grid(uint8_t offset, uint32_t color);
void draw_rect(int rx, int ry, int width, int height, uint32_t color);
void draw_line(int x0, int y0, int x1, int y1, uint32_t color);
void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

void my_draw_line(float x_start, float y_start, float x_end, float y_end, uint32_t color);
void my_draw_line2(float x0, float y0, float x1, float y1, uint32_t color);

#endif /* DISPLAY_H */
