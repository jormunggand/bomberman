UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)
CFLAGS = -I/opt/homebrew/include/SDL2 -D_THREAD_SAFE -Wall
LDFLAGS = -L/opt/homebrew/lib -lSDL2 -lSDL2_image
endif

ifeq ($(UNAME), Linux)
CFLAGS= -Wall -Werror
LDFLAGS = -lSDL2 -lSDL2_image
endif

CC = gcc

all: map
	./map

map: map.o utils.o
	$(CC) $(CFLAGS)	$^ -o $@ $(LDFLAGS)	

map_test: map_test.o map.o utils.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) && ./map_test
	rm $^ 

*.o: *.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -f *.o
	rm -f map map_test