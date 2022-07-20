#include "permutation.h"

// index into string provided by master cube for each port on the master
// encodes from, fromport, to, toport for each cube
const char codeidx[CZ] = {
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
const char ai[8*3] = {
1,3,4, // position 0, front left top
0,2,5, // position 1, back left top
1,3,6,
0,2,7,
0,5,7, // position 4, front left bottom
1,4,6, // position 5, back left bottom
2,5,7,
3,4,6
};



void print_positions(char v[]) {
	for (char i=0; i<LEN; i++)
		printf("%c", v[i] + '0');
	printf("\n");
}

void print_code(char code[]) {
	char i = 0;
	while(code[i] != '\0') {
		printf("%c",code[i]);
		++i;
	}
	printf("\n");
}

void print_matrix(char A[]) {
	printf("\n");
	for(int i=0; i<SZ; i++){
		for(int j=0; j<SZ; j++)
			printf("%c ", A[SZ*i+j] + '0');
		printf("\n");
	}
}

void code_to_adjacency_matrix(char A[], char code[]){
	for(char i = 0 ; i<SZ*SZ; i++)
		A[i]=0;
	char from, fromport, to, toport;
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



void config_to_adjacency_matrix(char A[], char cfg[]) {
	for(int i=0; i<SZ*SZ; i++)
		A[i] = 0;
	for(char i=0; i<LEN; i++){
		char col = cfg[i];
		for(char j=0; j<3; j++){
			char n = ai[3*i+j];
			char row = cfg[n];
			// hard-code outports for the master cube
			// otherwise put unknown port 6
			char v = 6;
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



void swap (char *x, char *y) {
	char temp;
	temp = *x;
	*x = *y;
	*y = temp;
}

// Now checks the complete cube
// In principle the function can return as soon as a single check fails
bool check_cube(char v[], char a[]){
	// construct adjacency matrix from v
	char b[SZ*SZ];
	config_to_adjacency_matrix(b, v);
	// and check if it equals a[]
#if DEBUG > 2
	print_positions(v);
#endif
	bool same = 1;
	for(char i=0; i<SZ;i++)	
		same &= (a[i] == b[i]);
	for(char i=SZ; i<SZ*SZ;i++)	
		same &= ((a[i]==0) == (b[i]==0));

	return same;
}


// Based on Heap's algorithm - see Wikipedia
// Can be much more efficient.
// Now also moves the master node around.
void adjacency_matrix_to_config(char v[], char A[]) {
	char c[LEN];
	for(char i=0; i<LEN; i++) {
		v[i] = i; // initialize to 0..7
		c[i] = 0;
	}

	if (check_cube(v, A))
		return;

	char i = 1;
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


void code_to_config(char cfg[], char code[]){
	char A[SZ*SZ];
	code_to_adjacency_matrix(A, code);
	adjacency_matrix_to_config(cfg, A);
}

#ifndef CUBE
int main() {
	char master[LEN] = {0,1,2,4,3,5,6,7};
	char currentconfig[LEN] = {0,1,2,3,4,5,6,7};
	char A[SZ*SZ];


// A1 E124 F514B1G3 H312D1G5
// A4 D315 H123G5E4 C213G1B4
// A5 B214 F145G3E2 C312G1D5
	char code[] = "A1E124F514B1G3H312D1G5A4D315H123G5E4C213G1B4A5B214F145G3E2C312G1D5";
	
	code_to_adjacency_matrix(A, code);
	print_matrix(A);

	config_to_adjacency_matrix(A, currentconfig);
	print_matrix(A);

	adjacency_matrix_to_config(master, A);
	print_positions(master);
	
	printf("Different code\n");
	char magic[LEN] = {0,7,5,6,3,2,4,1};
 	print_positions(magic);	
	config_to_adjacency_matrix(A, magic);
	print_matrix(A);

	adjacency_matrix_to_config(magic, A);
	print_positions(magic);

	char c[LEN] = {0,0,0,0,0,0,0,0};
	code_to_config(c, code);
	print_positions(c);

	return 0;

}
#endif
