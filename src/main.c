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
#include "../include/matrix.h"

// TODO: when filled triangle mesh get close enough it freezes becase there are missing vertices while filling triangles

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define LOGS(s) (printf("LOGS: %s\n", s))
#define LOGD(d) (printf("LOGD: %d\n", d))
#define LOGF(f) (printf("LOGF: %f\n", f))

#define PI (3.14159f)

bool is_running = false;
bool keys_down[255];
int delta_time = 0;

vec4_t **triangles_to_render;
int *triangles_to_render_backfaces = NULL;

cam_t camera = { 
  .translation = { .0f, .0f, .0f }, 
  .rotation = { .0f, .0f, .0f }, 
  .fov = PI/2.0f
};

mesh_t mesh = {
  .scale = { 1.0f, 1.0f, 1.0f },
  .translation = { 0.0f, 0.0f, 6.0f },
  .rotation = { 0.0f, 0.0f, 0.0f },
  .n_vertices = 0,
  .n_faces = 0,
  .vertices = NULL,
  .faces = NULL
};

enum cull_method cull_method;
enum render_method render_method;

void setup(void) {
  cull_method = CULL_BACKFACE;
  render_method = RENDER_WIRE;

  for (uint8_t i = 0; i < 255; i++) {
    keys_down[i] = false;
  }

  load_obj_file("assets/models/cube.obj", &mesh);

  triangles_to_render = malloc(sizeof(vec4_t*) * mesh.n_faces);
  triangles_to_render_backfaces = malloc(sizeof(int) * mesh.n_faces);

  uint32_t color = 0xFF787878;
  for (size_t i = 0; i < mesh.n_faces; i++) {
    triangles_to_render[i] = (vec4_t*)malloc(sizeof(vec4_t) * 3);
    triangles_to_render_backfaces[i] = 0;

    mesh.faces[i].color = color;

    color = color - 3000;
  }

  float aspect = (float)window_height / (float)window_width;
  float znear = 0.1f;
  float zfar = 100.0f;

  LOGF(aspect * (1.0f / tan(camera.fov / 2.0f)));
  LOGF((1.0f / tan(camera.fov / 2.0f)));
  LOGF(zfar / (zfar - znear));
  LOGF((-zfar *znear) / (zfar - znear));

  matrix_proj = mat4_make_perspective(camera.fov, aspect, znear, zfar);
}

void free_resources() {
  free(triangles_to_render);
  free(triangles_to_render_backfaces);
  free(mesh.vertices);
  free(mesh.faces);
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

        if (event.key.keysym.sym == SDLK_1) {
          keys_down[SDLK_1] = true;
          render_method = RENDER_WIRE;
        }

        if (event.key.keysym.sym == SDLK_2) {
          keys_down[SDLK_2] = true;
          render_method = RENDER_WIRE_VERTEX;
        }

        if (event.key.keysym.sym == SDLK_3) {
          keys_down[SDLK_3] = true;
          render_method = RENDER_FILL_TRIANGLE;
        }

        if (event.key.keysym.sym == SDLK_4) {
          keys_down[SDLK_4] = true;
          render_method = RENDER_FILL_TRIANGLE_WIRE;
        }

        if (event.key.keysym.sym == SDLK_c) {
          keys_down[SDLK_c] = true;
          cull_method = CULL_BACKFACE;
        }

        if (event.key.keysym.sym == SDLK_d) {
          keys_down[SDLK_d] = true;
          cull_method = CULL_NONE;
        }

        break;

      case SDL_KEYUP:
        if (event.key.keysym.sym == SDLK_w) {
          keys_down[SDLK_w] = false;
        }

        if (event.key.keysym.sym == SDLK_s) {
          keys_down[SDLK_s] = false;
        }

        if (event.key.keysym.sym == SDLK_c) {
          keys_down[SDLK_c] = false;
        }

        if (event.key.keysym.sym == SDLK_d) {
          keys_down[SDLK_d] = false;
        }

        if (event.key.keysym.sym == SDLK_1) {
          keys_down[SDLK_1] = false;
        }

        if (event.key.keysym.sym == SDLK_2) {
          keys_down[SDLK_2] = false;
        }

        if (event.key.keysym.sym == SDLK_3) {
          keys_down[SDLK_3] = false;
        }

          if (event.key.keysym.sym == SDLK_4) {
          keys_down[SDLK_4] = false;
        }

        break;
    }
  }
}

void update(void) {
  // process delta time frame

  //float angle = (SDL_GetTicks() + delta_time) / 2000.0f;

  mesh.rotation.x += ((float)delta_time)/2000.0f;
  mesh.rotation.y += ((float)delta_time)/2000.0f;
  mesh.rotation.z += ((float)delta_time)/1000.0f;

  mesh.scale.x += ((float)delta_time/9000.0f);
  mesh.scale.y += ((float)delta_time/6000.0f);

  //mesh.translation.x += ((float)delta_time/10000.0f);
  //mesh.translation.y += ((float)delta_time/1000.0f);
  //mesh.translation.z += ((float)delta_time/1000.0f);

  mat4_t matrix_scale = mat4_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);

  mat4_t matrix_rotation_x = mat4_rotate_x(mesh.rotation.x);
  mat4_t matrix_rotation_y = mat4_rotate_y(mesh.rotation.y);
  mat4_t matrix_rotation_z = mat4_rotate_z(mesh.rotation.z);

  mat4_t matrix_translation = mat4_translate(mesh.translation.x, mesh.translation.y, mesh.translation.z);

  for (size_t i = 0; i < mesh.n_faces; i++) {
    triangles_to_render_backfaces[i] = 0;

    face_t face = mesh.faces[i];

    vec4_t vertices[3];

    vertices[0] = vec4_from_vec3(mesh.vertices[face.a-1]);
    vertices[1] = vec4_from_vec3(mesh.vertices[face.b-1]);
    vertices[2] = vec4_from_vec3(mesh.vertices[face.c-1]);

    // Transformation of the vertices
    for (uint8_t j = 0; j < 3; j++) {
      vec4_t v = vertices[j];

      mat4_t matrix_world = mat4_identity();

      matrix_world = mat4_mul_mat4(matrix_scale, matrix_world);

      matrix_world = mat4_mul_mat4(matrix_rotation_x, matrix_world);
      matrix_world = mat4_mul_mat4(matrix_rotation_y, matrix_world);
      matrix_world = mat4_mul_mat4(matrix_rotation_z, matrix_world);

      matrix_world = mat4_mul_mat4(matrix_translation, matrix_world);

      v = mat4_mul_vec4(matrix_world, v);

      vertices[j] = v;
    }

    mesh.faces[i].depth = (vertices[0].z + vertices[1].z + vertices[2].z) / (float)3.0f;

    for (uint8_t j = 0; j < 3; j++) {
      vec4_t v_projected = mat4_mul_vec4_project(matrix_proj, vertices[j]);

      v_projected.x *= window_width/3.0f;
      v_projected.y *= window_height/3.0f;

      v_projected.x += window_width/2.0f;
      v_projected.y += window_height/2.0f;

      triangles_to_render[i][j] = v_projected;
    }

    vec2_t vertice_a; /*   A   */
    vec2_t vertice_b; /*  / \  */
    vec2_t vertice_c; /* C---B */

    vertice_a.x = triangles_to_render[i][0].x;
    vertice_a.y = triangles_to_render[i][0].y;

    vertice_b.x = triangles_to_render[i][1].x;
    vertice_b.y = triangles_to_render[i][1].y;

    vertice_c.x = triangles_to_render[i][2].x;
    vertice_c.y = triangles_to_render[i][2].y;
    
    vec2_t vector_ab = vec2_sub(vertice_b, vertice_a);
    vec2_t vector_ac = vec2_sub(vertice_c, vertice_a);

    float backface = vec2_cross(vector_ab, vector_ac);

    if (backface > 0 && cull_method == CULL_BACKFACE) {
      triangles_to_render_backfaces[i] = 1;
    }
  }

  //printf("%f\n", mesh.faces[3].depth);

  // sort faces by depth
  // because triangles_to_render and triangles_to_render_backfaces indexes are aligned same with mesh.faces sort them the same.
  for (size_t i = 0; i < mesh.n_faces; i++) {
    for (size_t j = 0; j < mesh.n_faces; j++) {
      if (j < (mesh.n_faces-1)) {
        face_t current_face = mesh.faces[j];
        face_t next_face = mesh.faces[j+1];

        vec4_t *current_tri = triangles_to_render[j];
        vec4_t *next_tri = triangles_to_render[j+1];

        int current_backface = triangles_to_render_backfaces[j];
        int next_backface = triangles_to_render_backfaces[j+1];

        if (current_face.depth < next_face.depth) {
          mesh.faces[j] = next_face;
          mesh.faces[j+1] = current_face;

          triangles_to_render[j] = next_tri;
          triangles_to_render[j+1] = current_tri;

          triangles_to_render_backfaces[j] = next_backface;
          triangles_to_render_backfaces[j+1] = current_backface;
        }
      }
    }
  }
}

void render(void) {
  // render projected coordinates
  color_buffer_clear(0xFF000000);

  draw_grid(24, 0x44444444);
  
  for (size_t i = 0; i < mesh.n_faces; i++) {
    if (triangles_to_render_backfaces[i] == 1) {
      continue;
    }

    vec4_t v0 = triangles_to_render[i][0];
    vec4_t v1 = triangles_to_render[i][1];
    vec4_t v2 = triangles_to_render[i][2];

    switch (render_method) {
      case RENDER_WIRE:
        draw_line(v0.x, v0.y, v1.x, v1.y, 0xFFFFFFFF);
        draw_line(v1.x, v1.y, v2.x, v2.y, 0xFFFFFFFF);
        draw_line(v2.x, v2.y, v0.x, v0.y, 0xFFFFFFFF);
        break;

      case RENDER_WIRE_VERTEX:
        draw_line(v0.x, v0.y, v1.x, v1.y, 0xFFFFFFFF);
        draw_line(v1.x, v1.y, v2.x, v2.y, 0xFFFFFFFF);
        draw_line(v2.x, v2.y, v0.x, v0.y, 0xFFFFFFFF);

        draw_rect(v0.x-3, v0.y-3, 6, 6, 0xFFFF0000);
        draw_rect(v1.x-3, v1.y-3, 6, 6, 0xFFFF0000);
        draw_rect(v2.x-3, v2.y-3, 6, 6, 0xFFFF0000);
        break;

      case RENDER_FILL_TRIANGLE:
        draw_filled_triangle(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y, mesh.faces[i].color);
        break;

      case RENDER_FILL_TRIANGLE_WIRE:
        draw_filled_triangle(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y, mesh.faces[i].color);
        draw_line(v0.x, v0.y, v1.x, v1.y, 0xFFFFFFFF);
        draw_line(v1.x, v1.y, v2.x, v2.y, 0xFFFFFFFF);
        draw_line(v2.x, v2.y, v0.x, v0.y, 0xFFFFFFFF);
        break;
    }
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
    //break;
  }

  window_destroy();
  free_resources();

  return 0;
}
