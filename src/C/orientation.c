#include <stdio.h>
#include <stdbool.h>
#include "orientation.h"

bool is_on_top(int8_t id) {
	int8_t set[4] = {0,1,2,3};
	bool inset = 0;
	for(int i=0; i<4; i++)
		inset |= (set[i]==id);
	return inset;
}


bool is_on_front(int8_t id) {
	int8_t set[4] = {0,3,4,7};
	bool inset = 0;
	for(int i=0; i<4; i++)
		inset |= (set[i]==id);
	return inset;
}


bool is_on_left(int8_t id) {
	int8_t set[4] = {0,1,4,5};
	bool inset = 0;
	for(int i=0; i<4; i++)
		inset |= (set[i]==id);
	return inset;
}

// canonical faces of a cube
// 1 points to top
// 4 to the right
// 2 to the front
const int8_t faces[6*3] = {
	0,0,1, // 1
	0,1,0, // 2
	-1,0,0, //3
	1,0,0, // 4
	0,-1,0, // 5
	0,0,-1 // 6
};



void inplace_dot(int8_t dice[], int8_t R[]) {
	int8_t out[9];
	for(int8_t row=0; row<3;row++) {
		for(int8_t col=0; col<3; col++) {
			int8_t sum = 0;
			for (int8_t i=0;i<3; i++)
			       sum += R[3*row+i] * dice[3*i+col];
			out[3*row + col] = sum;
		}
	}
	for(int8_t i=0; i<9; i++)
		dice[i] = out[i];
}

void RotateRx(int8_t dice[], int8_t n) {
	int8_t Rx[9] = {
	1,0,0,
	0,0,-1,
	0,1,0
	};
	for(int8_t i=0;i<n;i++)
		inplace_dot(dice,Rx);
}

void RotateRy(int8_t dice[], int8_t n) {
	int8_t Ry[9] = {
	0,0,1,
	0,1,0,
	-1,0,0
	};
	for(int8_t i=0;i<n;i++)
		inplace_dot(dice,Ry);
}

void RotateRz(int8_t dice[], int8_t n) {
	int8_t Rz[9] = {
	0,-1,0,
	1,0,0,
	0,0,1
	};
	for(int8_t i=0;i<n;i++)
		inplace_dot(dice,Rz);
}

void Mirrorx(int8_t dice[]) {
	int8_t Mx[9] = {
	-1,0,0,
	0,1,0,
	0,0,1
	};
	inplace_dot(dice,Mx);
}

void Mirrory(int8_t dice[]) {
	int8_t My[9] = {
	1,0,0,
	0,-1,0,
	0,0,1
	};
	inplace_dot(dice,My);
}

void Mirrorz(int8_t dice[]) {
	int8_t Mz[9] = {
	1,0,0,
	0,1,0,
	0,0,-1
	};
	inplace_dot(dice,Mz);
}

void print_dice(int8_t dice[]) {
	for (int8_t j=0;j<3;j++) {
		for(int8_t i=0;i<3;i++)
			printf("%c",dice[3*j + i] + '0');
		printf("\n");
	}
}

void print_ports(int8_t ports[]) {
	for (int8_t j=0;j<3;j++) {
		printf("%c", ports[j] + '0');
	}
		printf("\n");
}

void dice_to_port_numbers(int8_t dice[], int8_t ports[]) {
	for(int8_t col=0; col<3; col++) 
		for(int8_t i=0; i<6;i++) {
			bool found = true;
			for(int8_t row=0; row<3; row++) {
				found &= faces[3*i + row] == dice[3*row + col];
			}
			if (found)
				ports[col] = i+1;
		}
}


// in canonical state
void port_numbers_to_dice(int8_t dice[], int8_t ports[]) {
	for(int8_t p=0;p<3;p++)
		for (int8_t i=0; i<3; i++)
			dice[3*i+p] = faces[3*(ports[p]-1) + i];
}


// should we call this transform_to_position ?
void transform_to_position(int8_t dice[], int8_t position) {
	if( is_on_top(position) )
		Mirrorz(dice);
	if( !is_on_left(position) )
	       Mirrorx(dice);
	if( !is_on_front(position) )
		Mirrory(dice);	
}

void spin(int8_t ports[], int8_t position) {
	int8_t dice[9];
	port_numbers_to_dice(dice, ports);

	int r = rand()%4;
	RotateRz(dice, r);

	r = rand()%3;
	if( r == 0 ) {
		// Rx
// TODO: CHECK IF THIS IS CORRECT
		if (is_on_top(position) ^ !is_on_front(position)) {
			RotateRx(dice,1);
		} else {
			RotateRx(dice,3);
		}

	} else if (r == 1) {

		//Ry
		if (is_on_top(position) ^ !is_on_left(position)) {
			RotateRy(dice,3);
		} else {
			RotateRy(dice,1);
		}
	}
	// no rotation

	dice_to_port_numbers(dice, ports);
}

#ifndef CUBE
int main(void) {
	int8_t canonicaldice[9] = {
		0,1,0,
		0,0,1,
		1,0,0
	};
	int8_t ports[3];
	dice_to_port_numbers(canonicaldice, ports);
	printf("The matrix representation of the canonically oriented dice in world coordinates as used in the master code looks like:\n");
	print_dice(canonicaldice);
	printf("Ports (TopDown, LeftRight, FrontBack) that correspond to this dice in world coordinates are:\n");
	print_ports(ports);

	printf("\nIf the cube is constructed with the dice above in the bottom plane, and those dice rotated upwards around the x-axis in the top-plane,\nthen these are their respective connected ports.\nPosition: Ports");
	for( int8_t p=0; p<4;p++) {	
		printf("%c: ", p+'0');
		// TD LR FB
		int8_t dice[9] = {
			0,1,0,
			0,0,1,
			1,0,0
		};
		// top ones are flipped with 6 on top
		RotateRx(dice,2);
		transform_to_position(dice,p);
		dice_to_port_numbers(dice, ports);
		print_ports(ports);
	}
	// bottom dices are rotated in example
	for( int8_t p=4; p<8;p++) {	
		printf("%c: ", p+'0');
		// TD LR FB
		int8_t dice[9] = {
			0,1,0,
			0,0,1,
			1,0,0
		};
		transform_to_position(dice,p);
		dice_to_port_numbers(dice, ports);
		print_ports(ports);
	}
	
#if DEBUG > 3
	print_dice(canonicaldice);
	printf("\n");
	RotateRz(canonicaldice,1);
	print_dice(canonicaldice);
	printf("\n");
	RotateRz(canonicaldice,3);
	print_dice(canonicaldice);
#endif



}
#endif
