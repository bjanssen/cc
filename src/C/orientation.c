#include <stdio.h>
#include <stdbool.h>
#include "orientation.h"

bool is_on_top(char id) {
	char set[4] = {0,1,2,3};
	bool inset = 0;
	for(int i=0; i<4; i++)
		inset |= (set[i]==id);
	return inset;
}


bool is_on_front(char id) {
	char set[4] = {0,3,4,7};
	bool inset = 0;
	for(int i=0; i<4; i++)
		inset |= (set[i]==id);
	return inset;
}


bool is_on_left(char id) {
	char set[4] = {0,1,4,5};
	bool inset = 0;
	for(int i=0; i<4; i++)
		inset |= (set[i]==id);
	return inset;
}

// canonical faces of a cube
// 1 points to top
// 4 to the right
// 2 to the front
const char faces[6*3] = {
	0,0,1, // 1
	0,1,0, // 2
	-1,0,0, //3
	1,0,0, // 4
	0,-1,0, // 5
	0,0,-1 // 6
};



void inplace_dot(char dice[], char R[]) {
	char out[9];
	for(char row=0; row<3;row++) {
		for(char col=0; col<3; col++) {
			char sum = 0;
			for (char i=0;i<3; i++)
			       sum += R[3*row+i] * dice[3*i+col];
			out[3*row + col] = sum;
		}
	}
	for(char i=0; i<9; i++)
		dice[i] = out[i];
}

void RotateRx(char dice[], char n) {
	char Rx[9] = {
	1,0,0,
	0,0,-1,
	0,1,0
	};
	for(char i=0;i<n;i++)
		inplace_dot(dice,Rx);
}

void RotateRy(char dice[], char n) {
	char Ry[9] = {
	0,0,1,
	0,1,0,
	-1,0,0
	};
	for(char i=0;i<n;i++)
		inplace_dot(dice,Ry);
}

void RotateRz(char dice[], char n) {
	char Rz[9] = {
	0,-1,0,
	1,0,0,
	0,0,1
	};
	for(char i=0;i<n;i++)
		inplace_dot(dice,Rz);
}

void Mirrorx(char dice[]) {
	char Mx[9] = {
	-1,0,0,
	0,1,0,
	0,0,1
	};
	inplace_dot(dice,Mx);
}

void Mirrory(char dice[]) {
	char My[9] = {
	1,0,0,
	0,-1,0,
	0,0,1
	};
	inplace_dot(dice,My);
}

void Mirrorz(char dice[]) {
	char Mz[9] = {
	1,0,0,
	0,1,0,
	0,0,-1
	};
	inplace_dot(dice,Mz);
}

void print_dice(char dice[]) {
	for (char j=0;j<3;j++) {
		for(char i=0;i<3;i++)
			printf("%c",dice[3*j + i] + '0');
		printf("\n");
	}
}

void print_ports(char ports[]) {
	for (char j=0;j<3;j++) {
		printf("%c", ports[j] + '0');
	}
		printf("\n");
}

void dice_to_port_numbers(char dice[], char ports[]) {
	for(char col=0; col<3; col++) 
		for(char i=0; i<6;i++) {
			bool found = true;
			for(char row=0; row<3; row++) {
				found &= faces[3*i + row] == dice[3*row + col];
			}
			if (found)
				ports[col] = i+1;
		}
}


// in canonical state
void port_numbers_to_dice(char dice[], char ports[]) {
	for(char p=0;p<3;p++)
		for (char i=0; i<3; i++)
			dice[3*i+p] = faces[3*(ports[p]-1) + i];
}


// should we call this transform_to_position ?
void transform_to_position(char dice[], char position) {
	if( is_on_top(position) )
		Mirrorz(dice);
	if( !is_on_left(position) )
	       Mirrorx(dice);
	if( !is_on_front(position) )
		Mirrory(dice);	
}

void spin(char ports[], char position) {
	char dice[9];
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
	char canonicaldice[9] = {
		0,1,0,
		0,0,1,
		1,0,0
	};
	char ports[3];
	dice_to_port_numbers(canonicaldice, ports);
	printf("The matrix representation of the canonically oriented dice in world coordinates as used in the master code looks like:\n");
	print_dice(canonicaldice);
	printf("Ports (TopDown, LeftRight, FrontBack) that correspond to this dice in world coordinates are:\n");
	print_ports(ports);

	printf("\nIf the cube is constructed with the dice above in the bottom plane, and those dice rotated upwards around the x-axis in the top-plane,\nthen these are their respective connected ports.\nPosition: Ports");
	for( char p=0; p<4;p++) {	
		printf("%c: ", p+'0');
		// TD LR FB
		char dice[9] = {
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
	for( char p=4; p<8;p++) {	
		printf("%c: ", p+'0');
		// TD LR FB
		char dice[9] = {
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
