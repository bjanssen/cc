#ifndef PERMUTATION_H
#define PERMUTATION_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define LEN 8
#define SZ 8

//Links boven voor A met de klok mee BCD
//Onder A E en weer met de klok mee FGH
// bovenste zijde 6 naar boven
// onderste zijde 6 naar onder
// zijde 4 naar voren
// 1 boven, 4 naar voren, dan 2 rechts


// A1 E124 F514B1G3 H312D1G5
// A4 D315 H123G5E4 C213G1B4
// A5 B214 F145G3E2 C312G1D5
// A heeft of poort 1 E zitten 

//indices into incomming stringdata
// from, port, to, port
#define CZX 4
#define CZY 7
#define CZ  CZX*CZY
#define CZL 22

#define DEBUG 1
void code_to_config(char cfg[], char code[]);
void print_positions(char v[]);

#endif
