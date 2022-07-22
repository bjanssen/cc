#ifndef orientation_h
#define orientation_h
#include <stdbool.h>
#include <stdlib.h> // rand

void print_ports(int8_t ports[]);
void print_dice(int8_t dice[]);

void dice_to_port_numbers(int8_t dice[], int8_t ports[]);
void port_numbers_to_dice(int8_t dice[], int8_t ports[]);
void transform_to_position(int8_t dice[], int8_t position);
// to and from are their own inverses

void spin_bruteforce(int8_t ports[], int8_t position);
void spin(int8_t ports[], int8_t position);

#endif
