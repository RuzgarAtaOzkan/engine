#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128

#define FPS 30
#define MILLISECS_PER_FRAME (1000 / FPS)

bool window_create(void);
void window_destroy(void);

void framebuffer_clear(uint32_t color);
void framebuffer_render(void);

void framerate_fix(void);
void framerate_fix2(int *dt_global);

void draw_pixel(uint8_t x, uint8_t y, uint32_t color);

#endif
