#include "permutation.h"

// index into string provided by master cube for each port on the master
// encodes from, fromport, to, toport for each cube
const int8_t codeidx[CZ] = {
0, 1, 2, 3,
2, 4, 6, 7,
2, 5, 14, 15,
6, 8, 10, 11,
6, 9, 12, 13,
14, 16, 18, 19,
14, 17, 20, 21
};

// Adjacency index map
// Map from cube position to its 3 neighbours
const int8_t ai[8*3] = {
1,3,4, // position 0, front left top
0,2,5, // position 1, back left top
1,3,6,
0,2,7,
0,5,7, // position 4, front left bottom
1,4,6, // position 5, back left bottom
2,5,7,
3,4,6
};



void print_positions(int8_t v[]) {
	for (int8_t i=0; i<LEN; i++)
		printf("%c", v[i] + '0');
	printf("\n");
}

void print_code(int8_t code[]) {
	int8_t i = 0;
	while(code[i] != '\0') {
		printf("%c",code[i]);
		++i;
	}
	printf("\n");
}

void print_matrix(int8_t A[]) {
	printf("\n");
	for(int i=0; i<SZ; i++){
		for(int j=0; j<SZ; j++)
			printf("%c ", A[SZ*i+j] + '0');
		printf("\n");
	}
}

void code_to_adjacency_matrix(int8_t A[], int8_t code[]){
	for(int8_t i = 0 ; i<SZ*SZ; i++)
		A[i]=0;
	int8_t from, fromport, to, toport;
	for(int j=0; j<3; j++)
		for(int i=0; i<7; i++) {
			from = code[j*CZL + codeidx[CZX*i]] - 'A'; 
			fromport = code[j*CZL + codeidx[CZX*i + 1]] - '0'; 
			to = code[j*CZL + codeidx[CZX*i + 2]] - 'A'; 
			toport = code[j*CZL + codeidx[CZX*i + 3]] - '0'; 

			A[SZ*from + to] = fromport;
			A[SZ*to + from] = toport;
		}
}



void config_to_adjacency_matrix(int8_t A[], int8_t cfg[]) {
	for(int i=0; i<SZ*SZ; i++)
		A[i] = 0;
	for(int8_t i=0; i<LEN; i++){
		int8_t col = cfg[i];
		for(int8_t j=0; j<3; j++){
			int8_t n = ai[3*i+j];
			int8_t row = cfg[n];
			// hard-code outports for the master cube
			// otherwise put unknown port 6
			int8_t v = 6;
			if(row==0) {
				if (i==1)
					v = 5;
				else if(i==3)
					v = 4;
				else if(i==4)
					v = 1;
			} else {
				v = -1;
			}
			A[SZ*row + col] = v;	
		}
	}
}



void swap (int8_t *x, int8_t *y) {
	int8_t temp;
	temp = *x;
	*x = *y;
	*y = temp;
}

// Now checks the complete cube
// In principle the function can return as soon as a single check fails
bool check_cube(int8_t v[], int8_t a[]){
	// hack to not check invalid codes
	// master is always at position 0
	if (v[0] != 0)
		return false;
	// construct adjacency matrix from v
	int8_t b[SZ*SZ];
	config_to_adjacency_matrix(b, v);
	// and check if it equals a[]
#if DEBUG > 2
	print_positions(v);
#endif
	bool same = 1;
	for(int8_t i=0; i<SZ;i++)	
		same &= (a[i] == b[i]);
	for(int8_t i=SZ; i<SZ*SZ;i++)	
		same &= ((a[i]==0) == (b[i]==0));

	return same;
}


// Based on Heap's algorithm - see Wikipedia
// Can be much more efficient.
// Now also moves the master node around.
void adjacency_matrix_to_config(int8_t v[], int8_t A[]) {
	int8_t c[LEN];
	for(int8_t i=0; i<LEN; i++) {
		v[i] = i; // initialize to 0..7
		c[i] = 0;
	}

	if (check_cube(v, A))
		return;

	int8_t i = 1;
	while( i<LEN ){
		if(c[i] < i){
			if(i % 2 == 0) {
				swap(&v[0], &v[i]);
			} else {
				swap(&v[c[i]], &v[i]);
			}
			if (check_cube(v, A))
				return;
			c[i] += 1;
			i = 1;

		} else {
			c[i] = 0;
			i += 1;
		}
	}
}


void code_to_config(int8_t cfg[], int8_t code[]){
	int8_t A[SZ*SZ];
	code_to_adjacency_matrix(A, code);
	adjacency_matrix_to_config(cfg, A);
}

int8_t n_correct_positions(int8_t master[], int8_t state[]) {
	int8_t n=0;
	for(int8_t i=0; i<LEN; i++)
		n += (master[i] == state[i]);
	return n;
}

#ifndef CUBE
int main() {
	int8_t master[LEN] = {0,1,2,4,3,5,6,7};
	int8_t currentconfig[LEN] = {0,1,2,3,4,5,6,7};
	int8_t A[SZ*SZ];


// A1 E124 F514B1G3 H312D1G5
// A4 D315 H123G5E4 C213G1B4
// A5 B214 F145G3E2 C312G1D5
	int8_t code[] = "A1E124F514B1G3H312D1G5A4D315H123G5E4C213G1B4A5B214F145G3E2C312G1D5";
	printf("Code received on Master:\n%s\n", code);
	
	code_to_adjacency_matrix(A, code);
	printf("Adjacency matrix constructed from code:\n");
	print_matrix(A);

	config_to_adjacency_matrix(A, currentconfig);
	printf("Position information mapped to adjacency matrix:\n");
	print_matrix(A);

	adjacency_matrix_to_config(master, A);
	printf("Cube positions computed from adjacency matrix\n");
	print_positions(master);
	
	printf("Different permutation of cube positions\n");
	int8_t magic[LEN] = {0,7,5,6,3,2,4,1};
 	print_positions(magic);	
	config_to_adjacency_matrix(A, magic);
	print_matrix(A);

	adjacency_matrix_to_config(magic, A);
	printf("Reconstruction based on adjacency matrix.\n");
	print_positions(magic);
	
	// check code_to_config
	int8_t c[LEN] = {0,0,0,0,0,0,0,0};
	code_to_config(c, code);
	print_positions(c);

	return 0;

}
#endif
