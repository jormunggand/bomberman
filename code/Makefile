CFLAGS= -Wall -Werror

make: test.o

test.o: test.c
	gcc $(CFLAGS) -o test.o test.c -lSDL2

clean: test.o
	rm test.o
