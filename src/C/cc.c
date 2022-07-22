#include "cube.h"
#include "orientation.h"
#include <stdio.h>

struct cube {
	int8_t pos[LEN];
	int8_t orientation[3*LEN]; // TD LR FB
};

int main() {
// A1 E124 F514B1G3 H312D1G5
// A4 D315 H123G5E4 C213G1B4
// A5 B214 F145G3E2 C312G1D5
	int8_t code[] = "A1E124F514B1G3H312D1G5A4D315H123G5E4C213G1B4A5B214F145G3E2C312G1D5";
	printf("Code received by on master:\n%s\n\n", code);
	Cube master = {.pos = {0,1,2,3,4,5,6,7},
		.orientation = {6,4,5,
			6,4,5,
			6,4,5,
			6,4,5,
			1,4,2,
			1,4,2,
			1,4,2,
			1,4,2}
	};
	Cube state;
#if DEBUG > 2
	print_positions(master.pos);
	print_ports(master.orientation);
	int8_t dice[9];
	port_numbers_to_dice(dice, master.orientation);
	print_dice(dice);
	transform_to_position(dice,0);
	dice_to_port_numbers(dice, master.orientation);
	print_ports(master.orientation);

	port_numbers_to_dice(dice, master.orientation);
	transform_to_position(dice,0);
	dice_to_port_numbers(dice, master.orientation);
	print_ports(master.orientation);
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
			
	Cube random_state = generate_random_cube();
	printf("\n--Random state---\n");
	print_state(random_state);
	printf("-----\n");
	printf("Number of correct positions: %c\n", n_correct_positions(master.pos, random_state.pos) + '0');
	printf("Number of correct orientations: %c\n", n_correct_orientations(master, random_state) + '0');
}
