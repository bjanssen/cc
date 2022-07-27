#include "cube.h"

struct cube {
	int8_t pos[LEN];
	int8_t orientation[3*LEN]; // TD LR FB
};

int8_t n_correct_orientations(Cube master, Cube state) {
	int8_t n = 0;
	for(int8_t i=0; i<LEN; i++) {
		int8_t p = 0;
		// find correspondence between master and state
		while( master.pos[p] != state.pos[i])
			p++;
		bool same = true;
		for(int8_t j=0; j<3; j++)
			same &= (master.orientation[3*p+j] == state.orientation[3*i+j]);
		n += same;
	}
	return n;
}

// UD:0 LR:1, FB:2
int8_t n_correct_orientation_zxy(Cube master, Cube state, int8_t o) {
	int8_t n = 0;
	for(int8_t i=0; i<LEN; i++) {
		int8_t p = 0;
		// find correspondence between master and state
		while( master.pos[p] != state.pos[i])
			p++;
		bool same = true;
		same &= (master.orientation[3*p+o] == state.orientation[3*i+o]);
		n += same;
	}
	return n;
}

void print_state(Cube state) {
	printf("pos: ");
	for(int8_t i=0; i<LEN; i++)
		printf("%c", state.pos[i] + '0');
	printf("\nUD : ");
	for(int8_t i=0; i<LEN; i++)
		printf("%c", state.orientation[3*i] + '0');
	printf("\nLR : ");
	for(int8_t i=0; i<LEN; i++)
		printf("%c", state.orientation[3*i+1] + '0');
	printf("\nFB : ");
	for(int8_t i=0; i<LEN; i++)
		printf("%c", state.orientation[3*i+2] + '0');
	printf("\n");
}

void print_reordered_state(Cube state) {
	printf("pos: ");
	for(int8_t i=0; i<LEN; i++)
		printf("%c", state.pos[i] + 'A');
	printf("\nUD : ");
	for(int8_t i=0; i<LEN; i++)
		printf("%c", state.orientation[3*state.pos[i]] + '0');
	printf("\nLR : ");
	for(int8_t i=0; i<LEN; i++)
		printf("%c", state.orientation[3*state.pos[i]+1] + '0');
	printf("\nFB : ");
	for(int8_t i=0; i<LEN; i++)
		printf("%c", state.orientation[3*state.pos[i]+2] + '0');
	printf("\n");
}

// At what index should we look to find respectively
// TopDown LeftRight FrontBack
const int8_t TDLRFB[3*8] = {
	4,3,1, // 0 top left front
	5,2,0, // 1
	6,1,3, // 2
	7,0,2, // 3
	0,7,5, // 4 bottom
	1,6,4, // 5
	2,5,7, // 6
	3,4,6  // 7
};

void code_to_state(Cube *state, int8_t code[]){
	int8_t p;
	int8_t dice[9];
	int8_t A[SZ*SZ];
	int8_t v[3];
	code_to_config(state->pos, code);
	code_to_adjacency_matrix(A, code);

	
	for(int8_t i=0; i<LEN; i++) {
		// transform back to worldcoordinates
		// based on actual position
		// at position i the cube labelled p is positioned
		p = state->pos[i];
		// read ports from adjacency matrix
		// based on durrent postion in the cube
		for(int8_t j=0; j<3; j++){
			int8_t idx = state->pos[TDLRFB[3*p+j]];
			v[j] = A[SZ*p + idx];
			//printf("%c", v[j] + '0');
		}
		port_numbers_to_dice(dice, v);
		transform_to_position(dice,i);
		dice_to_port_numbers(dice, &(state->orientation[3*p]));
	}
}

// from: https://stackoverflow.com/questions/6127503/shuffle-array-in-c
void shuffle(int8_t *array, size_t n)
{
    if (n > 1) 
    {
        size_t i;
        for (i = 0; i < n - 1; i++) 
        {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          int8_t t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}


void spin_all(Cube *state) {

	for(int8_t i=1;i<LEN; i++) {
		int8_t p = state->pos[i];
		spin( &(state->orientation[3*p]), i);	
	}
}


Cube generate_random_cube(){
	// This cube can be modified by arbitrarily applying Rz rotations
	// followed by a position dependent Rx or Ry
	Cube master = {.pos = {0,1,2,3,4,5,6,7},
		.orientation = {
			6,4,5,// pre-rotate 0 to fixed position
			//1,4,2,
			1,4,2,
			1,4,2,
			1,4,2,
			1,4,2,
			1,4,2,
			1,4,2,
			1,4,2}
	};
	shuffle(&(master.pos[1]), LEN-1); // hacky way to leave 0 untouched
	// Don't rotate the first dice
#if DEBUG > 2
	print_state(master);
#endif
	spin_all(&master);

	return master;
}


