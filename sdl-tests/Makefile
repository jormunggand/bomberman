CFLAGS= -Wall -Werror


%.o: %.c
	gcc $(CFLAGS) -o $@ $^ -lSDL2 -lSDL2_image

clean: 
	rm *.o
