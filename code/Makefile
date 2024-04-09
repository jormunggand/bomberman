CFLAGS= -Wall -Werror


%.o: %.c utils.c
	gcc $(CFLAGS) -o $@ $^ -lSDL2 -lSDL2_image

clean: 
	rm *.o
