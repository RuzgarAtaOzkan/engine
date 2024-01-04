#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <SDL.h>

#include "vector.h"
#include "display.h"
#include "utils.h"
#include "triangle.h"

void my_draw_line(float x_start, float y_start, float x_end, float y_end, uint32_t color);
void my_draw_line2(float x0, float y0, float x1, float y1, uint32_t color);

void *f_obj_load(FILE *file, char c, size_t *length);

int fac(int n);

#endif /* UTILS_H */
