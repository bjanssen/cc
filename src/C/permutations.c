#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define LEN 7

void swap (char *x, char *y) {
	char temp;
	temp = *x;
	*x = *y;
	*y = temp;
}


// placeholder
bool check_cube(char v[], char a[]){
	// construct adjacency matrix from v
	// and check if it equals a[]
	for (char i=0; i<LEN; i++)
		printf("%d", v[i]);
	printf("\n");
	return 0;
}


// Based on Heap's algorithm - see Wikipedia
void traverse_cube_configurations_nonrecursive(char v[]) {
	char c[LEN];
	for(char i=0; i<LEN; i++)
		c[i] = 0;

	if (print(v))
		return;

	char i = 1;
	while( i<LEN ){
		if(c[i] < i){
			if(i % 2 == 0) {
				swap(&v[0], &v[i]);
			} else {
				swap(&v[c[i]], &v[i]);
			}
			if (print(v))
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
	char num[LEN] = {1,2,3,4,5,6,7};
	char len = LEN;

	traverse_cube_configurations_nonrecursive(num);
	print(num);

	return 0;

}
