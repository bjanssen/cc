#include "cube.h"

struct cube {
	char pos[LEN];
	char or[3*LEN]; // TD LR FB
};

char n_correct_orientations(Cube master, Cube state) {
	char n = 0;
	for(char i=0; i<LEN; i++) {
		char p = 0;
		// find correspondence between master and state
		while( master.pos[p] != state.pos[i])
			p++;
		bool same = true;
		for(char j=0; j<3; j++)
			same &= (master.or[3*p+j] == state.or[3*i+j]);
		n += same;
	}
	return n;
}

// UD:0 LR:1, FB:2
char n_correct_orientation(Cube master, Cube state, char o) {
	char n = 0;
	for(char i=0; i<LEN; i++) {
		char p = 0;
		// find correspondence between master and state
		while( master.pos[p] != state.pos[i])
			p++;
		bool same = true;
		same &= (master.or[3*p+o] == state.or[3*i+o]);
		n += same;
	}
	return n;
}

void print_state(Cube state) {
	printf("pos: ");
	for(char i=0; i<LEN; i++)
		printf("%c", state.pos[i] + '0');
	printf("\nUD : ");
	for(char i=0; i<LEN; i++)
		printf("%c", state.or[3*i] + '0');
	printf("\nLR : ");
	for(char i=0; i<LEN; i++)
		printf("%c", state.or[3*i+1] + '0');
	printf("\nFB : ");
	for(char i=0; i<LEN; i++)
		printf("%c", state.or[3*i+2] + '0');
	printf("\n");
}

// At what index should we look to find respectively
// TopDown LeftRight FrontBack
const char TDLRFB[3*8] = {
	4,3,1, // 0 top left front
	5,2,0, // 1
	6,1,3, // 2
	7,0,2, // 3
	0,7,5, // 4 bottom
	1,6,4, // 5
	2,5,7, // 6
	3,4,6  // 7
};

void code_to_state(Cube *state, char code[]){
	char p;
	char dice[9];
	char A[SZ*SZ];
	char v[3];
	code_to_config(state->pos, code);
	code_to_adjacency_matrix(A, code);

	
	for(char i=0; i<LEN; i++) {
		// transform back to worldcoordinates
		// based on actual position
		// at position i the cube labelled p is positioned
		p = state->pos[i];
		// read ports from adjacency matrix
		// based on durrent postion in the cube
		for(char j=0; j<3; j++){
			char idx = state->pos[TDLRFB[3*p+j]];
			v[j] = A[SZ*p + idx];
			//printf("%c", v[j] + '0');
		}
		port_numbers_to_dice(dice, v);
		transform_to_position(dice,i);
		dice_to_port_numbers(dice, &(state->or[3*p]));
	}
}

// from: https://stackoverflow.com/questions/6127503/shuffle-array-in-c
void shuffle(char *array, size_t n)
{
    if (n > 1) 
    {
        size_t i;
        for (i = 0; i < n - 1; i++) 
        {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          char t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}



Cube generate_random_cube(){
	// This cube can be modified by arbitrarily applying Rz rotations
	// followed by a position dependent Rx or Ry
	Cube master = {.pos = {0,1,2,3,4,5,6,7},
		.or = {6,4,5,
			6,4,5,
			6,4,5,
			6,4,5,
			1,4,2,
			1,4,2,
			1,4,2,
			1,4,2}
	};
	shuffle(&(master.pos[1]), LEN-1); // hacky way to leave 0 untouched
	// Don't rotate the first dice
	for(char i=1;i<LEN; i++)
	       spin( &(master.or[3*i]), i);	

	return master;
}


#ifndef CUBE

int main() {
// A1 E124 F514B1G3 H312D1G5
// A4 D315 H123G5E4 C213G1B4
// A5 B214 F145G3E2 C312G1D5
	char code[] = "A1E124F514B1G3H312D1G5A4D315H123G5E4C213G1B4A5B214F145G3E2C312G1D5";
	printf("Code received by on master:\n%s\n\n", code);
	Cube master = {.pos = {0,1,2,3,4,5,6,7},
		.or = {6,4,5,
			6,4,5,
			6,4,5,
			6,4,5,
			1,4,2,
			1,4,2,
			1,4,2,
			1,4,2}
	};
	Cube state;
	Cube rstate = generate_random_cube(); 
	print_state(rstate);
#if DEBUG > 2
	print_positions(master.pos);
	print_ports(master.or);
	char dice[9];
	port_numbers_to_dice(dice, master.or);
	print_dice(dice);
	transform_to_position(dice,0);
	dice_to_port_numbers(dice, master.or);
	print_ports(master.or);

	port_numbers_to_dice(dice, master.or);
	transform_to_position(dice,0);
	dice_to_port_numbers(dice, master.or);
	print_ports(master.or);
	printf("========\n");
#endif

	//code_to_config(state.pos, code);
	code_to_state(&state, code);

	printf("--State of master code--\n");
	print_state(master);
	printf("--State based on example string---\n");
	print_state(state);
	printf("-----\n");
	printf("Number of correct positions: %c\n", n_correct_positions(master.pos, state.pos) + '0');
	printf("Number of correct orientations: %c\n", n_correct_orientations(master, state) + '0');
			
}
#endif
