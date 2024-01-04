#include "../include/mesh.h"

vec3_t mesh_vertices[N_MESH_VERTICES] = {
  { -1, -1, -1 },
  { -1,  1, -1 },
  {  1,  1, -1 },
  {  1, -1, -1 },
  {  1,  1,  1 },
  {  1, -1,  1 },
  { -1,  1,  1 },
  { -1, -1,  1 },
};

face_t mesh_faces[N_MESH_FACES] = {
  // front 
  { .a = 1, .b = 2, .c = 3 },
  { .a = 1, .b = 3, .c = 4 },
  //right 
  { .a = 4, .b = 3, .c = 5 },
  { .a = 4, .b = 5, .c = 6 },
  // back
  { .a = 6, .b = 5, .c = 7 },
  { .a = 6, .b = 7, .c = 8 },
  // left
  { .a = 8, .b = 7, .c = 2 },
  { .a = 8, .b = 2, .c = 1 },
  // top
  { .a = 2, .b = 7, .c = 5 },
  { .a = 2, .b = 5, .c = 3 },
  // bottom
  { .a = 6, .b = 8, .c = 1 },
  { .a = 6, .b = 1, .c = 4 },
};
