#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "triangle.h"

#define N_MESH_VERTICES 8
#define N_MESH_FACES (6*2)

typedef struct {
  vec3_t *vertices;
  face_t *faces;
  vec3_t rotation;
} mesh_t;

extern vec3_t mesh_vertices[N_MESH_VERTICES];
extern face_t mesh_faces[N_MESH_FACES];

#endif /* MESH_H */
