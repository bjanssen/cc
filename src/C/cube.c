#include "permutation.h"
#include "orientation.h"

typedef struct Cube {
	char pos[LEN];
	char or[3*LEN]; // TD LR FB
} Cube;

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

	code_to_config(state.pos, code);
	print_positions(state.pos);
}
