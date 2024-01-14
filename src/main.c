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

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define LOGS(s) (printf("LOGS: %s\n", s))
#define LOGD(d) (printf("LOGD: %d\n", d))
#define LOGF(f) (printf("LOGF: %f\n", f))

#define PI (3.14159f)

bool is_running = false;
bool keys_down[255];

int delta_time = 0;

cam_t camera = { 
  .position = { .0f, .0f, .0f }, 
  .rotation = { .0f, .0f, .0f }, 
  .fov = 90
};

vec2_t **triangles_to_render;
int *triangles_to_render_backfaces = NULL;

mesh_t obj_mesh = {
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

  load_obj_file("assets/models/cube.obj", &obj_mesh);

  triangles_to_render = malloc(sizeof(vec2_t*) * obj_mesh.n_faces);
  triangles_to_render_backfaces = malloc(sizeof(int) * obj_mesh.n_faces);

  uint32_t color = 0xFF787878;
  for (size_t i = 0; i < obj_mesh.n_faces; i++) {
    triangles_to_render[i] = (vec2_t*)malloc(sizeof(vec2_t) * 3);
    triangles_to_render_backfaces[i] = 0;

    obj_mesh.faces[i].color = color;

    color = color - 1000;
  }
}

void free_resources() {
  free(triangles_to_render);
  free(triangles_to_render_backfaces);
  free(obj_mesh.vertices);
  free(obj_mesh.faces);
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
          render_method = RENDER_WIRE_VERTEX;
        }

        if (event.key.keysym.sym == SDLK_2) {
          keys_down[SDLK_2] = true;
          render_method = RENDER_WIRE;
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

  obj_mesh.rotation.x += ((float)delta_time)/2000.0f;
  obj_mesh.rotation.y += ((float)delta_time)/2000.0f;
  obj_mesh.rotation.z += ((float)delta_time)/1000.0f;

  for (size_t i = 0; i < obj_mesh.n_faces; i++) {
    triangles_to_render_backfaces[i] = 0;
    face_t face = obj_mesh.faces[i];

    vec3_t face_vertices[3];
    vec3_t transformed_vertices[3];

    face_vertices[0] = obj_mesh.vertices[face.a-1];
    face_vertices[1] = obj_mesh.vertices[face.b-1];
    face_vertices[2] = obj_mesh.vertices[face.c-1];

    // Transformation of the vertices
    for (size_t j = 0; j < 3; j++) {
      vec3_t v = face_vertices[j];

      v = vec3_rotate_x(v, obj_mesh.rotation.x);
      v = vec3_rotate_y(v, obj_mesh.rotation.y);
      v = vec3_rotate_z(v, obj_mesh.rotation.z);

      v.z += 5;

      transformed_vertices[j] = v;
    }

    //////////////////////////////////////////////
    // Apply Backface culling check
    //////////////////////////////////////////////
    /*
    vec3_t vertice_a = transformed_vertices[0];    A   
    vec3_t vertice_b = transformed_vertices[1];   / \  
    vec3_t vertice_c = transformed_vertices[2];  C---B 

    vec3_t vector_ab = vec3_sub(vertice_b, vertice_a);
    vec3_t vector_ac = vec3_sub(vertice_c, vertice_a);

    vec3_t normal = vec3_cross(vector_ab, vector_ac);

    vec3_t camera_ray = vec3_sub(camera.position, vertice_a);

    float dot_normal_camera = vec3_dot(normal, camera_ray);

    if (dot_normal_camera < 0.00f) {
      triangles_to_render_backfaces[i] = 1;
      continue;
    }
    */

    ///////////////////////////////////////////////////////
    // vec3 ==> vec2
    // Project 3d vectors to 2d vectors
    //////////////////////////////////////////////////////
    for (size_t j = 0; j < 3; j++) {
      vec3_t v = transformed_vertices[j];

      vec2_t v_projected = project(v, 256*3);

      v_projected.x += window_width/2;
      v_projected.y += window_height/2;

      triangles_to_render[i][j] = v_projected;
    }

    vec2_t vertice_a = triangles_to_render[i][0]; /*   A   */
    vec2_t vertice_b = triangles_to_render[i][1]; /*  / \  */
    vec2_t vertice_c = triangles_to_render[i][2]; /* C---B */

    vec2_t vector_ab = vec2_sub(vertice_b, vertice_a);
    vec2_t vector_ac = vec2_sub(vertice_c, vertice_a);

    float backface = vec2_cross(vector_ab, vector_ac);

    //float backface = triangle_edge_cross(&vertice_a, &vertice_b, &vertice_c);

    if (backface > 0 && cull_method == CULL_BACKFACE) {
      triangles_to_render_backfaces[i] = 1;
    }
  }
}

void render(void) {
  // render projected coordinates
  color_buffer_clear(0xFF000000);

  draw_grid(16, 0x55555555);
  
  for (size_t i = 0; i < obj_mesh.n_faces; i++) {
    if (triangles_to_render_backfaces[i] == 1) {
      continue;
    }

    vec2_t v0 = triangles_to_render[i][0];
    vec2_t v1 = triangles_to_render[i][1];
    vec2_t v2 = triangles_to_render[i][2];

    //draw_rect(v0.x, v0.y, 32, 32, 0xFFFF0000);
    //draw_rect(v1.x, v1.y, 32, 32, 0xFF00FF00);
    //draw_rect(v2.x, v2.y, 32, 32, 0xFF0000FF);

    //fill_triangle(v0, v1, v2, 0xFF0000FF);

    //draw_line(300, 100, 50, 400, 0xFFFFFFFF);
    //draw_line(50, 400, 500, 700, 0xFFFFFFFF);
    //draw_line(500, 700, 300, 100, 0xFFFFFFFF);

    //draw_filled_triangle(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y, 0xFFFFFFFF);

    if (render_method == RENDER_WIRE_VERTEX) {
      draw_line(v0.x, v0.y, v1.x, v1.y, 0xFFFFFFFF);
      draw_line(v1.x, v1.y, v2.x, v2.y, 0xFFFFFFFF);
      draw_line(v2.x, v2.y, v0.x, v0.y, 0xFFFFFFFF);

      draw_rect(v0.x-3, v0.y-3, 6, 6, 0xFFFF0000);
      draw_rect(v1.x-3, v1.y-3, 6, 6, 0xFFFF0000);
      draw_rect(v2.x-3, v2.y-3, 6, 6, 0xFFFF0000);
    } else if (render_method == RENDER_WIRE) {
      draw_line(v0.x, v0.y, v1.x, v1.y, 0xFFFFFFFF);
      draw_line(v1.x, v1.y, v2.x, v2.y, 0xFFFFFFFF);
      draw_line(v2.x, v2.y, v0.x, v0.y, 0xFFFFFFFF);
    } else if (render_method == RENDER_FILL_TRIANGLE) {
      draw_filled_triangle(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y, obj_mesh.faces[i].color);
    } else if (render_method == RENDER_FILL_TRIANGLE_WIRE) {
      draw_filled_triangle(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y, obj_mesh.faces[i].color);

      draw_line(v0.x, v0.y, v1.x, v1.y, 0xFFFFFFFF);
      draw_line(v1.x, v1.y, v2.x, v2.y, 0xFFFFFFFF);
      draw_line(v2.x, v2.y, v0.x, v0.y, 0xFFFFFFFF);
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
