cube: cc.c cube.o orientation.o permutation.o
	gcc -g -o cube cube.c orientation.o permutation.o

orientation: orientation.h orientation.c
	gcc -g -o orientation orientation.c -DDEBUG=4

permutation: permutation.c permutation.h
	gcc -g -o permutation permutation.c


orientation.o: orientation.h orientation.c
	gcc -g -c orientation.c -DCUBE -DDEBUG=1

permutation.o: permutation.c permutation.h
	gcc -g -c permutation.c -DCUBE

cube.o: cube.c cube.h
	gcc -g -c cube.c -DCUBE

clean:
	rm cube orientation permutation orientation.o permutation.o cube.o
