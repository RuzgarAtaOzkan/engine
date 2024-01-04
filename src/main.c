#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <SDL.h>

#include "../include/display.h"
#include "../include/utils.h"
#include "../include/vector.h"
#include "../include/mesh.h"

#define PI 3.141592

bool is_running = false;
bool *keys_down = NULL;

int delta_time = 0;

vec3_t cam_pos = { .x = 0, .y = 0, .z = -5 };
triangle_t triangles_to_render[N_MESH_FACES];
vec2_t **my_triangles_to_render;

size_t f22_v_len;
size_t f22_f_len;
vec3_t *f22_v = NULL;
face_t *f22_f = NULL;

void setup(void) {
  keys_down = (bool*)malloc(sizeof(bool)*255);
  for (uint8_t i = 0; i < 255; i++) {
    keys_down[i] = false;
  }

  FILE *f22;
  f22 = fopen("assets/models/f22.obj", "r");

  f22_v = (vec3_t*)f_obj_load(f22, 'v', &f22_v_len);
  f22_f = (face_t*)f_obj_load(f22, 'f', &f22_f_len);

  if (f22_v == NULL || f22_f == NULL) {
    return;
  }

  my_triangles_to_render = (vec2_t**)malloc(sizeof(vec2_t) * f22_f_len);

  for (size_t i = 0; i < f22_f_len; i++) {
    my_triangles_to_render[i] = (vec2_t*)malloc(sizeof(vec2_t) * 3);
  }

  fclose(f22);
}

void input_process(void) {
  SDL_Event event;
  
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        is_running = false;
        break;
      case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          is_running = false;
        }

        if (event.key.keysym.sym == SDLK_w) {
          keys_down[SDLK_w] = true;
        }

        if (event.key.keysym.sym == SDLK_s) {
          keys_down[SDLK_s] = true;
        }

        break;

      case SDL_KEYUP:
        if (event.key.keysym.sym == SDLK_w) {
          keys_down[SDLK_w] = false;
        }

        if (event.key.keysym.sym == SDLK_s) {
          keys_down[SDLK_s] = false;
        }

        break;
    }
  }
}

void update(void) {
  // process delta time frame

  float angle = (SDL_GetTicks() + delta_time) / 1000.0f;

  for (size_t i = 0; i < f22_f_len; i++) {
    face_t face = f22_f[i];

    vec3_t vertices[3];

    vertices[0] = f22_v[face.a-1];
    vertices[1] = f22_v[face.b-1];
    vertices[2] = f22_v[face.c-1];

    for (size_t j = 0; j < 3; j++) {
      vec3_t v = vertices[j];

      v = vec3_rotate_x(v, (vec3_t){ 0.0f, 0.0f, 0.0f }, angle);
      v = vec3_rotate_y(v, (vec3_t){ 0.0f, 0.0f, 0.0f }, angle);
      v = vec3_rotate_z(v, (vec3_t){ 0.0f, 0.0f, 0.0f }, angle);

      v.z -= cam_pos.z;

      vec2_t proj = project(v, 256*3);

      proj.x += window_width/2;
      proj.y += window_height/2;

      my_triangles_to_render[i][j] = proj;
    }
  }

  /**
    for (int i = 0; i < N_MESH_FACES; i++) {
    face_t mesh_face = mesh_faces[i];

    vec3_t face_vertices[3];

    face_vertices[0] = mesh_vertices[mesh_face.a-1];
    face_vertices[1] = mesh_vertices[mesh_face.b-1];
    face_vertices[2] = mesh_vertices[mesh_face.c-1];

    for (int j = 0; j < 3; j++) {
      vec3_t vertex = face_vertices[j];

      vertex = vec3_rotate_x(vertex, (vec3_t){ .0f, .0f, .0f }, angle);
      vertex = vec3_rotate_y(vertex, (vec3_t){ .0f, .0f, .0f }, angle);
      vertex = vec3_rotate_z(vertex, (vec3_t){ .0f, .0f, .0f }, angle);

      vertex.z -= cam_pos.z;
      
      vec2_t projected = project(vertex, 768);

      projected.x += window_width/2;
      projected.y += window_height/2;

      my_triangles_to_render[i][j] = projected;
    }
  }
  */


  /*
    for (int i = 0; i < N_MESH_VERTICES; i++) {
      vec3_t vertice = mesh_vertices[i];    
      vec3_t rotated = vec3_rotate_x(vertice, (vec3_t){ .0f, .0f, .0f }, angle);
      rotated = vec3_rotate_y(rotated, (vec3_t){ .0f, .0f, .0f }, angle);
      rotated = vec3_rotate_z(rotated, (vec3_t){ .0f, .0f, .0f }, angle);
      rotated.z -= cam_pos.z;
      vec2_t projected = project(rotated, 650);
      projected.x += window_width/2;
      projected.y += window_height/2;
      vertices_to_render[i] = projected;
    }
  */
}

void render(void) {
  // render projected coordinates
  color_buffer_clear(0xFF000000);

  
  
    for (int i = 0; i < f22_f_len; i++) {

    vec2_t vertex0 = my_triangles_to_render[i][0];
    vec2_t vertex1 = my_triangles_to_render[i][1];
    vec2_t vertex2 = my_triangles_to_render[i][2];

    //draw_rect(vertex0.x, vertex0.y, 3, 3, 0xFFFFFFFF);
    //draw_rect(vertex1.x, vertex1.y, 3, 3, 0xFFFFFFFF);
    //draw_rect(vertex2.x, vertex2.y, 3, 3, 0xFFFFFFFF);

    draw_line(vertex0.x, vertex0.y, vertex1.x, vertex1.y, 0xFF00FF00);
    draw_line(vertex1.x, vertex1.y, vertex2.x, vertex2.y, 0xFF00FF00);
    draw_line(vertex2.x, vertex2.y, vertex0.x, vertex0.y, 0xFF00FF00);
  }

  

  color_buffer_render();
}

int main(int argc, char **argv) {
  is_running = window_init();

  setup();

  while (is_running) {
    framerate_fix(&delta_time);
    input_process();
    update();
    render();
  }

  window_destroy();

  return 0;
}
