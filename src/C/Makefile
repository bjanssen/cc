cube: cube.c orientation.o permutation.o
	gcc -g -o cube cube.c orientation.o permutation.o -DCUBE

orientation: orientation.h orientation.c
	gcc -g -o orientation orientation.c

permutation: permutation.c permutation.h
	gcc -g -o permutation permutation.c


orientation.o: orientation.h orientation.c
	gcc -g -c orientation.c -DCUBE

permutation.o: permutation.c permutation.h
	gcc -g -c permutation.c -DCUBE


clean:
	rm cube orientation permutation orientation.o permutation.o 
