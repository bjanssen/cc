#ifndef CUBE_H
#define CUBE_H

#include "permutation.h"
#include "orientation.h"
#include <stdlib.h> // rand


typedef struct cube Cube;


int8_t n_correct_orientations(Cube master, Cube state);
int8_t n_correct_orientation_zxy(Cube master, Cube state, int8_t o);
void code_to_state(Cube *state, int8_t code[]);
Cube generate_random_cube();
void print_state(Cube state);
#endif
