#ifndef orientation_h
#define orientation_h
#include <stdbool.h>
#include <stdlib.h> // rand

void print_ports(char ports[]);
void print_dice(char dice[]);

void dice_to_port_numbers(char dice[], char ports[]);
void port_numbers_to_dice(char dice[], char ports[]);
void transform_to_position(char dice[], char position);
// to and from are their own inverses

void spin(char ports[], char position);

#endif
