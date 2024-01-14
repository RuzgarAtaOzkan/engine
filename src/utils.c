#include "../include/vector.h"
#include "../include/mesh.h"
#include "../include/display.h"
#include "../include/utils.h"

int fac(int n) {
  int r = 1;
  for (int i = 1; i <= n; i++) {
    r = r * i;
  }

  return r;
}

float cos_to_sine(float c) {
  float x = c;
  float y = sqrt(1.00f - (x*x));
  return y;
}

void *f_obj_load(const char *filepath, const char c, size_t *length) {
  FILE *file;
  file = fopen(filepath, "r");

  if (file == NULL) {
    return NULL;
  }

  uint8_t i_linecc = 0; // line char check index

  if (c != 'v' && c != 'f') {
    i_linecc = 1;
  }

  uint8_t n_line = 64;
  uint8_t n_num = 32;

  char line[n_line]; // current line buffer
  char num[n_num]; // number buffer

  for (size_t i = 0; i < n_line; i++) {
    line[i] = '\0';
  }

  for (size_t j = 0; j < n_num; j++) {
    num[j] = '\0';
  }

  // length of the array that will be allocated
  size_t n_lines = 0;
  while (fgets(line, sizeof(line) / sizeof(line[0]), file)) {
    // lines starts with -> v
    if (line[i_linecc] == c && line[i_linecc+1] == ' ') {
      n_lines++;
    }
  }

  *length = n_lines;
  size_t i_output = 0;
  void *output = NULL;
  
  switch (c) {
    case 'v':
    case 'n':
    case 't':
      output = malloc(sizeof(vec3_t)*n_lines);
      break;
    case 'f':
      output = malloc(sizeof(face_t)*n_lines);
      break;
  }

  fseek(file, 0, SEEK_SET);

  while (fgets(line, sizeof(line) / sizeof(line[0]), file)) {
    // lines starts with -> v
    if (line[i_linecc] != c || line[i_linecc+1] != ' ') {
      // go to next line if line doesnt start with given character and space
      // e.g. v 0.123 -0.322 1.3223
      continue;
    }

    uint8_t i_num = 0;
    uint8_t i_xyz = 0;
    bool num_read = false;

    for (size_t i = 0; i < n_line; i++) {
      if (line[i] == '\n') {
        // z value
        //printf("%f\n", atof(num));

        if (c != 'f') {
          ((vec3_t*)output)[i_output].z = atof(num);
        } else {
          ((face_t*)output)[i_output].c = atoi(num);
        }

        // clear num char array
        for (size_t j = 0; j < n_num; j++) {
          num[j] = '\0';
        }

        break;
      }
      
      if (line[i] == ' ') {
        if (i_xyz == 1) {
          // x value
          //printf("%f\n", atof(num));

          if (c != 'f') {
            ((vec3_t*)output)[i_output].x = atof(num);
          } else {
            ((face_t*)output)[i_output].a = atoi(num);
          }
        }

        if (i_xyz == 2) {
          // y value
          //printf("%f\n", atof(num));
          if (c != 'f') {
            ((vec3_t*)output)[i_output].y = atof(num);
          } else {
            ((face_t*)output)[i_output].b = atoi(num);
          }
        }

        // clear num char array
        for (size_t j = 0; j < n_num; j++) {
          num[j] = '\0';
        }

        i_num = 0;
        i_xyz++;
        num_read = true;

        continue;
      }

      if (line[i] == '/') {
        num_read = false;
        continue;
      }

      if (num_read) {
        num[i_num] = line[i];
        i_num++;
      }
    }

    // clear line char array
    for (size_t i = 0; i < n_line; i++) {
      line[i] = '\0';
    }

    i_output++;
  }

  fseek(file, 0, SEEK_SET);
  fclose(file);

  return output;
}

void load_obj_file(const char *filepath, mesh_t *mesh) {
  FILE *file;
  file = fopen(filepath, "r");

  if (file == NULL) {
    return;
  }

  uint8_t n_line = 64;
  char line[n_line]; // current line buffer

  for (uint8_t i = 0; i < n_line; i++) {
    line[i] = '\0';
  }

  // length of the array that will be allocated
  size_t n_vertices = 0;
  size_t n_faces = 0;
  while (fgets(line, n_line, file)) {
    if (strncmp(line, "v ", 2) == 0) {
      n_vertices++;
    }

    if (strncmp(line, "f ", 2) == 0) {
      n_faces++;
    }
  }

  mesh->n_vertices = n_vertices;
  mesh->n_faces = n_faces;
  mesh->vertices = (vec3_t*)malloc(sizeof(vec3_t)*n_vertices);
  mesh->faces = (face_t*)malloc(sizeof(face_t)*n_faces);

  for (uint8_t i = 0; i < n_line; i++) {
    line[i] = '\0';
  }

  fseek(file, 0, SEEK_SET);

  size_t i_vertices = 0;
  size_t i_faces = 0;
  while (fgets(line, n_line, file)) {
    if (strncmp(line, "v ", 2) == 0) {
      vec3_t vertex;
      sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
      mesh->vertices[i_vertices] = vertex;
      i_vertices++;
    }

    if (strncmp(line, "f ", 2) == 0) {
      face_t face;
      int textures[3];
      int normals[3];

      if (strchr(line, '/') == NULL) {
        sscanf(line, "f %d %d %d", &face.a, &face.b, &face.c);
      } else {
        sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", 
          &face.a, &textures[0], &normals[0], 
          &face.b, &textures[1], &normals[1], 
          &face.c, &textures[2], &normals[2]
        );
      }

      mesh->faces[i_faces] = face;
      i_faces++;
    }

    for (uint8_t i = 0; i < n_line; i++) {
      line[i] = '\0';
    }
  }

  fseek(file, 0, SEEK_SET);
  fclose(file);
}
