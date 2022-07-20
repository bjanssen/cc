#include "cube.h"

struct cube {
	char pos[LEN];
	char or[3*LEN]; // TD LR FB
};

char n_correct_orientations(Cube master, Cube state) {
	char n = 0;
	char p;
	for(char i=0; i<LEN; i++) {
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
	char p;
	for(char i=0; i<LEN; i++) {
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
		printf("\n");
		for(char j=0; j<3; j++){
			char idx = state->pos[TDLRFB[3*p+j]];
			v[j] = A[SZ*p + idx];
			printf("%c", v[j] + '0');
		}
		printf("->");
		port_numbers_to_dice(dice, v);
		transform_to_position(dice,i);
		dice_to_port_numbers(dice, &(state->or[3*p]));
		print_ports(&(state->or[3*p]));
	}
}
		



int main() {
// A1 E124 F514B1G3 H312D1G5
// A4 D315 H123G5E4 C213G1B4
// A5 B214 F145G3E2 C312G1D5
	char code[] = "A1E124F514B1G3H312D1G5A4D315H123G5E4C213G1B4A5B214F145G3E2C312G1D5";
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


	//code_to_config(state.pos, code);
	code_to_state(&state, code);
	print_positions(state.pos);

	printf("-----\n");
	print_state(master);
	printf("-----\n");
	print_state(state);
	printf("-----\n");
	printf("Number of correct positions: %c\n", n_correct_positions(master.pos, state.pos) + '0');
	printf("Number of correct orientations: %c\n", n_correct_orientations(master, state) + '0');
			
}
