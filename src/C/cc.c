#include "cube.h"
#include "orientation.h"
#include <stdio.h>

int main() {
// A1 E124 F514B1G3 H312D1G5
// A4 D315 H123G5E4 C213G1B4
// A5 B214 F145G3E2 C312G1D5
	int8_t code[] = "A1E124F514B1G3H312D1G5A4D315H123G5E4C213G1B4A5B214F145G3E2C312G1D5";
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
#if DEBUG > 2
	print_positions(master.pos);
	print_ports(master.or);
	int8_t dice[9];
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
