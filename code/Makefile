CC = gcc
CFLAGS = -I/opt/homebrew/include/SDL2 -D_THREAD_SAFE -Wall
LDFLAGS = -L/opt/homebrew/lib -lSDL2 -lSDL2_image

all: map
	./map

map: map.o utils.o
	$(CC) $(CFLAGS)	$^ -o $@ $(LDFLAGS)	

*.o: *.c
	$(CC) $(CFLAGS) -c $^