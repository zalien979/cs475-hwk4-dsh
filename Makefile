all: builtins.o dsh.o queue.o
	gcc -Wall -g -o dsh main.c builtins.o dsh.o queue.o

builtins.o: builtins.h builtins.c
	gcc -Wall -g -c builtins.c

queue.o: queue.h queue.c
	gcc -Wall -g -c queue.c

dsh.o: dsh.h dsh.c
	gcc -Wall -g -c dsh.c
clean:
	rm -f dsh *.o
