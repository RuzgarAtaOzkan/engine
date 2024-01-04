#include "../include/display.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *color_buffer_texture = NULL;

uint32_t *color_buffer = NULL;

int window_width = 800;
int window_height = 600;

bool window_init(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL.\n");
    return false;
  }

  SDL_DisplayMode display_mode;
  SDL_GetCurrentDisplayMode(0, &display_mode);

  window_width = display_mode.w;
  window_height = display_mode.h;

  window = SDL_CreateWindow(
    NULL, 
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    window_width,
    window_height,
    SDL_WINDOW_BORDERLESS 
  );

  if (!window) {
    fprintf(stderr, "Error creating SDL window.\n");
    return false;
  }

  renderer = SDL_CreateRenderer(window, -1, 0);

  if (!renderer) {
    fprintf(stderr, "Error creating SDL renderer.\n");
    return false;
  }

  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

  color_buffer = (uint32_t*)malloc(window_width*window_height * sizeof(uint32_t));

  if (color_buffer == NULL) {
    fprintf(stderr, "Error allocating collor_buffer.\n");
    return false;
  }

  color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, window_width, window_height);

  if (color_buffer_texture == NULL) {
    fprintf(stderr, "Error creating SDL texture.\n");
    return false;
  }

  return true;
}

void window_destroy(void) {
  free(color_buffer);
  SDL_DestroyTexture(color_buffer_texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void color_buffer_render(void) {
  SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer, (int)(window_width) * sizeof(uint32_t));
  SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
  SDL_RenderPresent(renderer);
}

void color_buffer_clear(uint32_t color) {
  for (size_t i = 0; i < window_width*window_height; i++) {
    color_buffer[i] = color;
  }
}

void framerate_fix(int *dt) {
  static int end = 0;
  int dt_local = SDL_GetTicks() - end;

  if (dt_local > 0 && dt_local < MS_PER_FRAME) {
    SDL_Delay(MS_PER_FRAME - dt_local);
  }

 // delta time (the time passed between a frame interval)
  *dt = SDL_GetTicks() - end;
  end = SDL_GetTicks();
}

void draw_pixel(int x, int y, uint32_t color) {
  if (x >= 0 && x < window_width && y >= 0 && y < window_height) {
    color_buffer[(window_width*y) + x] = color;
  }
}

void draw_grid(uint8_t offset) {
  for (size_t y = 0; y < window_height; y++) {
    for (size_t x = 0; x < window_width; x++) {
      if (x % offset == 0 || y % offset == 0) {
        draw_pixel(x, y, 0xFF333333);
      }
    }
  }
}

void draw_rect(int rx, int ry, int width, int height, uint32_t color) {
  for (int y = ry; y < ry + height; y++) {
    for (int x = rx; x < rx + width; x++) {
      draw_pixel(x, y, color);
    }
  }
}

void draw_line(int x0, int y0, int x1, int y1, uint32_t color) {
  int dx = x1-x0;
  int dy = y1-y0;

  int long_side = (abs(dx) >= abs(dy)) ? abs(dx) : abs(dy);

  float x_inc = dx / (float)long_side;
  float y_inc = dy / (float)long_side;

  float cx = x0;
  float cy = y0;
  for (int i = 0; i < long_side; i++) {
    draw_pixel(round(cx), round(cy), color);
    cx = cx + x_inc;
    cy = cy + y_inc;
  }
}

