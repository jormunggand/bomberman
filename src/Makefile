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

all: main.exe
	./main.exe

map: map.o utils.o
	$(CC) $(CFLAGS)	$^ -o $@ $(LDFLAGS)	

map_test: map_test.o map.o utils.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) && ./map_test
	rm -f *.o

main.exe: main.o utils.o map.o move.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	rm -f *.o

*.o: *.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -f *.o

realclean: clean
	rm -f map map_test main.exe
