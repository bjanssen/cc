#ifndef CUBE_H
#define CUBE_H

#include "permutation.h"
#include "orientation.h"


typedef struct cube Cube;


char n_correct_orientations(Cube master, Cube state);
char n_correct_orientation(Cube master, Cube state, char o);
void code_to_state(Cube *state, char code[]);

#endif
