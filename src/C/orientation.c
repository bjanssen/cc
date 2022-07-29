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

// Mirror in the plane orthogonal to the 2-5 axis
void Mirrorx(int8_t dice[]) {
	for(int8_t i=0; i<3; i++)
		dice[3*i+1] *= -1;
}

// Mirror in the plane orthogonal to the 3-4 axis
void Mirrory(int8_t dice[]) {
	for(int8_t i=0; i<3; i++)
		dice[3*i+2] *= -1;
}

// Mirror in the plane orthogonal to the 1-6 axis
void Mirrorz(int8_t dice[]) {
	for(int8_t i=0; i<3; i++)
		dice[3*i] *= -1;
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


// same as above, but then in port representation
void transform_ports(int8_t ports[], int8_t position) {
	if( is_on_top(position) )
		ports[0] = 7 - ports[0];
	if( !is_on_left(position) )
		ports[1] = 7 - ports[1];
	if( !is_on_front(position) )
		ports[2] = 7 - ports[2];
}


bool check_dice(int8_t dice[], int8_t position) {
	int8_t ports[3];
	transform_to_position(dice, position);
	dice_to_port_numbers(dice, ports);
	bool contains_six = false;
	for(int8_t i=0; i<3; i++)
		contains_six |= (ports[i] == 6);
#if DEBUG > 2
	if( contains_six ) {
		printf("Port on position %d should not contain a 6\n", position);
		print_ports(ports);
	}
#endif
	// transform back
	transform_to_position(dice, position);
	return !contains_six;
}

// Brute force version because of representation bug
void spin_bruteforce(int8_t ports[], int8_t position) {
	int8_t dice[9];
	port_numbers_to_dice(dice, ports);

	int r = rand();
	if ((r%100) > 25)
		RotateRz(dice, 1);
	r = rand();
	if ((r%100) > 75)
		RotateRx(dice, 1);
	r = rand();
	if ((r%100) > 75)
		RotateRy(dice, 1);
	// if not a proper dice, try again
	while(!check_dice(dice, position)) {
		if ((r%100) > 25)
			RotateRz(dice, 1);
		r = rand();
		if ((r%100) > 75)
			RotateRx(dice, 1);
		r = rand();
		if ((r%100) > 75)
			RotateRy(dice, 1);
	}

	dice_to_port_numbers(dice, ports);
}


bool check_ports(int8_t ports[], int8_t ref[]) {
	bool correct = true;
	for (int8_t i = 0; i<3; i++)
		correct &= (ports[i] == ref[i]);
	return correct;
}

void reset_ports(int8_t ports[]) {
	ports[0]=1;
	ports[1]=4;
	ports[2]=2;
}
	

void spin(int8_t ports[], int8_t position) {
	int8_t dice[9];

	reset_ports(ports);
	port_numbers_to_dice(dice, ports);
	
	/*
	int8_t ref[] = {1,4,2};
	if(!check_ports(ports, ref)){
		printf("ERROR: input port for position %d not id: ", position);
		print_ports(ports);
	}
	*/

	// move to default position
	if( is_on_top(position) )
		RotateRx(dice, 2);

	int r = rand()%3;
#if DEBUG > 4
	if( is_on_top(position) ^ !is_on_left(position) ) {
		// Rx
		RotateRx(dice,1);
		printf("%d:%d%d%d  Rx %d\n",position, is_on_top(position), is_on_front(position), is_on_left(position), check_dice(dice, position));
		RotateRx(dice,3);
	} else {
		// -Rx
		RotateRx(dice,3);
		printf("%d:%d%d%d -Rx %d\n",position, is_on_top(position), is_on_front(position), is_on_left(position), check_dice(dice, position));
		RotateRx(dice,1);
	}
#endif

#if DEBUG > 4
	if( !is_on_top(position) ) {
		// Ry
		RotateRy(dice,1);
		printf("%d:%d%d%d  Ry %d\n",position, is_on_top(position), is_on_front(position), is_on_left(position), check_dice(dice, position));
		RotateRy(dice,3);
	} else {
		// -Ry
		RotateRy(dice,3);
		printf("%d:%d%d%d -Ry %d\n",position, is_on_top(position), is_on_front(position), is_on_left(position), check_dice(dice, position));
		RotateRy(dice,1);
	}
#endif


	if( r == 0 ) {
		// Rx
		// Probably some bug in the ordering
		// FB and LR are swapped
		// So hacky way of doing correct rotation
		if( is_on_top(position) ^ !is_on_left(position) ) {
			// Rx
			RotateRx(dice,1);
		} else {
			// -Rx
			RotateRx(dice,3);
		}

	} else if (r == 1) {
		// Ry
		if( !is_on_top(position) ) {
			// Ry
			RotateRy(dice,1);
		} else {
			// -Ry
			RotateRy(dice,3);
		}
	}
	// no additional rotation
	
	// spin around 1-6 axis
	r = rand()%4;
	RotateRz(dice, r);
#if DEBUG > 0
	check_dice(dice, position);
#endif

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

	print_dice(canonicaldice);
	printf("\nMz\n");
	Mirrorz(canonicaldice);
	dice_to_port_numbers(canonicaldice, ports);
	print_ports(ports);
	print_dice(canonicaldice);
	Mirrorz(canonicaldice);
	print_dice(canonicaldice);
	dice_to_port_numbers(canonicaldice, ports);
	print_ports(ports);

	printf("\n\nMy\n");
	Mirrory(canonicaldice);
	dice_to_port_numbers(canonicaldice, ports);
	print_ports(ports);
	print_dice(canonicaldice);
	Mirrory(canonicaldice);
	print_dice(canonicaldice);
	dice_to_port_numbers(canonicaldice, ports);
	print_ports(ports);

	printf("\n\nMx\n");
	Mirrorx(canonicaldice);
	dice_to_port_numbers(canonicaldice, ports);
	print_ports(ports);
	print_dice(canonicaldice);
	Mirrorx(canonicaldice);
	print_dice(canonicaldice);
	dice_to_port_numbers(canonicaldice, ports);
	print_ports(ports);
	
#if DEBUG > 4
	print_dice(canonicaldice);
	printf("\nRx\n");
	RotateRx(canonicaldice,1);
	dice_to_port_numbers(canonicaldice, ports);
	print_ports(ports);
	print_dice(canonicaldice);
	RotateRx(canonicaldice,3);
	print_dice(canonicaldice);

	printf("\nRy\n");
	RotateRy(canonicaldice,1);
	dice_to_port_numbers(canonicaldice, ports);
	print_ports(ports);
	print_dice(canonicaldice);
	RotateRy(canonicaldice,3);
	print_dice(canonicaldice);

	printf("\nRz\n");
	RotateRz(canonicaldice,1);
	dice_to_port_numbers(canonicaldice, ports);
	print_ports(ports);
	print_dice(canonicaldice);
	RotateRz(canonicaldice,3);
	print_dice(canonicaldice);

	printf("----\n");
	print_dice(canonicaldice);
	printf("\nRxRz");
	RotateRz(canonicaldice,1);
	RotateRx(canonicaldice,1);
	dice_to_port_numbers(canonicaldice, ports);
	print_ports(ports);
	print_dice(canonicaldice);
	RotateRx(canonicaldice,3);
	RotateRz(canonicaldice,3);

	printf("\nRyRz\n");
	RotateRz(canonicaldice,1);
	RotateRy(canonicaldice,1);
	dice_to_port_numbers(canonicaldice, ports);
	print_ports(ports);
	print_dice(canonicaldice);
	RotateRy(canonicaldice,3);
	RotateRz(canonicaldice,3);

	printf("\nRzRz\n");
	RotateRz(canonicaldice,1);
	RotateRz(canonicaldice,1);
	dice_to_port_numbers(canonicaldice, ports);
	print_ports(ports);
	print_dice(canonicaldice);
	RotateRz(canonicaldice,2);

	printf("----\n");
	print_dice(canonicaldice);
	printf("\nRzRxRx");
	RotateRx(canonicaldice,2);
	RotateRz(canonicaldice,1);
	dice_to_port_numbers(canonicaldice, ports);
	print_ports(ports);
	print_dice(canonicaldice);
	RotateRz(canonicaldice,3);
	RotateRx(canonicaldice,2);

	printf("\nRyRxRx\n");
	RotateRx(canonicaldice,2);
	RotateRy(canonicaldice,1);
	dice_to_port_numbers(canonicaldice, ports);
	print_ports(ports);
	print_dice(canonicaldice);
	RotateRy(canonicaldice,3);
	RotateRx(canonicaldice,2);

#endif



}
#endif
