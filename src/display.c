#include "../include/display.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

uint32_t *framebuffer = NULL;

SDL_Texture *framebuffer_texture = NULL;

uint16_t window_height = 900;
uint16_t window_width = 900;

bool window_create(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL.\n");
    return false;
  }

  window = SDL_CreateWindow("engine.c", 0, 0, window_width, window_height, SDL_WINDOW_OPENGL);

  if (!window) {
    fprintf(stderr, "Error creating SDL window.\n");
    return false;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

  if (!renderer) {
    fprintf(stderr, "Error creating SDL renderer.\n");
    return false;
  }

  framebuffer = malloc(sizeof(uint32_t) * SCREEN_WIDTH * SCREEN_HEIGHT);

  framebuffer_texture = SDL_CreateTexture(
    renderer,
    SDL_PIXELFORMAT_RGBA32,
    SDL_TEXTUREACCESS_STREAMING,
    SCREEN_WIDTH,
    SCREEN_HEIGHT
  );

  return true;
}

void draw_pixel(uint8_t x, uint8_t y, uint32_t color) {
  if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) {
    return;
  }

  framebuffer[(SCREEN_WIDTH * y) + x] = color;
}

void framebuffer_render(void) {
  SDL_UpdateTexture(
    framebuffer_texture,
    NULL,
    framebuffer,
    (int)(SCREEN_WIDTH * sizeof(uint32_t))
  );

  SDL_RenderCopy(renderer, framebuffer_texture, NULL, NULL);
  SDL_RenderPresent(renderer);
}

void framebuffer_clear(uint32_t color) {
  for (uint16_t i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
    framebuffer[i] = color;
  }
}

void framerate_fix(void) {
  static int32_t  previous_frame_time = 0;
  int time_ellapsed = SDL_GetTicks() - previous_frame_time;
  int time_to_wait = MILLISECS_PER_FRAME - time_ellapsed;

  if (time_to_wait > 0 && time_to_wait < MILLISECS_PER_FRAME) {
    SDL_Delay(time_to_wait);
  }
  
  previous_frame_time = SDL_GetTicks();
}

void framerate_fix2(int *dt_global) {
  static int start = 0;
  int dt = SDL_GetTicks() - start;

  if (dt > 0 && dt < MILLISECS_PER_FRAME) {
    SDL_Delay(MILLISECS_PER_FRAME - dt);
  }

  int current = SDL_GetTicks();

  *dt_global = current - start;
  start = current;
}

void window_destroy(void) {
  free(framebuffer);
  SDL_DestroyTexture(framebuffer_texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
