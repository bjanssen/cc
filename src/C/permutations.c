#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define LEN 10
#define SZ 10

//Links boven voor A met de klok mee BCD
//Onder A E en weer met de klok mee FGH
// bovenste zijde 6 naar boven
// onderste zijde 6 naar onder
// zijde 4 naar voren
// 1 boven, 4 naar voren, dan 2 rechts


// A1 E124 F514B1G3 H312D1G5
// A4 D315 H123G5E4 C213G1B4
// A5 B214 F145G3E2 C312G1D5
// A heeft of poort 1 E zitten 

//indices into incomming stringdata
// from, port, to, port
#define CISZ 28
const char codeidx[CISZ] = {0, 1, 2, 3,
2, 4, 6, 7,
2, 5, 14, 15,
6, 8, 10, 11,
6, 9, 12, 13,
14, 16, 18, 19,
14, 17, 20, 21};

const char ai[8*3] = {
1,3,4,
0,2,5,
1,3,6,
8,2,7,
9,5,7,
1,4,6,
2,5,7,
3,4,6
};


void print_matrix(char A[]) {
	for(int i=0; i<SZ; i++){
		for(int j=0; j<SZ; j++)
			printf("%c ", A[SZ*i+j] + '0');
		printf("\n");
	}
}

void code_to_adjacency_matrix(char A[], char code[]){
	char i = 0;
	while(code[i] != '\0') {
		printf("%c",code[i]);
		++i;
	}
	printf("\n");
	for(char i = 0 ; i<SZ*SZ; i++)
		A[i]=0;
	char from, fromport, to, toport;
	for(int j=0; j<3; j++)
		for(int i=0; i<7; i++) {
			from = code[j*22 + codeidx[4*i]] - 'A'; 
			fromport = code[j*22 + codeidx[4*i + 1]] - '0'; 
			to = code[j*22 + codeidx[4*i + 2]] - 'A'; 
			toport = code[j*22 + codeidx[4*i + 3]] - '0'; 
			if(j>0) {
				if(from==0)
					from = 7+j;
				if(to==0)
					to = 7+j;
			}

			A[SZ*from + to] = fromport;
			A[SZ*to + from] = toport;
		}
}

void config_to_adjacency_matrix(char A[], char cfg[]) {
	for(int i=0; i<SZ*SZ; i++)
		A[i] = 0;
	for(char i=0; i<LEN; i++){
		char row = cfg[i];
		for(char j=0; j<3; j++){
			char n;
			if(i>7 || i==0) {
			if(i==0) n = ai[0];
			if(i==7) n = ai[1];
			if(i==8) n = ai[2];
			} else {
				n = ai[3*i+j];
			}
			char col = cfg[n];
		printf("%c", col + '0');
			A[10*row + col] = 1;	
			A[10*col + row] = 1;	
		}
	}
}


void swap (char *x, char *y) {
	char temp;
	temp = *x;
	*x = *y;
	*y = temp;
}

// placeholder
bool check_cube(char v[], char a[]){
	// construct adjacency matrix from v
	char b[SZ*SZ];
	config_to_adjacency_matrix(b, v);
	// and check if it equals a[]
	for (char i=0; i<LEN; i++)
		printf("%c", v[i] + '0');
	printf("\n");
	bool same = 1;
	for(char i=0; i<SZ*SZ;i++)	
		same &= (a[i] == b[i]);

	return same;
}


// Based on Heap's algorithm - see Wikipedia
void adjacency_matrix_to_config(char v[], char A[]) {
	char c[LEN];
	for(char i=0; i<LEN; i++)
		c[i] = 0;

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




int main() {
	char master[LEN] = {0,1,2,4,3,5,6,7,8,9};
	char currentconfig[LEN] = {0,1,2,3,4,5,6,7,8,9};
	char A[SZ*SZ];


// A1 E124 F514B1G3 H312D1G5
// A4 D315 H123G5E4 C213G1B4
// A5 B214 F145G3E2 C312G1D5
	char code[] = "A1E124F514B1G3H312D1G5A4D315H123G5E4C213G1B4A5B214F145G3E2C312G1D5";
	
	code_to_adjacency_matrix(A, code);
	print_matrix(A);

	check_cube(currentconfig, A);

	config_to_adjacency_matrix(A, currentconfig);
	print_matrix(A);
	//adjacency_matrix_to_config(master, A);

	return 0;

}
