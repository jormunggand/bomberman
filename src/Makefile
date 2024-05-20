UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)
CFLAGS = -I/opt/homebrew/include/SDL2 -D_THREAD_SAFE -Wall -g
LDFLAGS = -L/opt/homebrew/lib -lSDL2 -lSDL2_image -lSDL2_net -lSDL2_ttf
endif

ifeq ($(UNAME), Linux)
CFLAGS= -Wall -Werror
LDFLAGS= -lSDL2 -lSDL2_image -lSDL2_net -lSDL2_ttf -lm
endif

CC = gcc

all: main.exe
	./main.exe

main.exe: main.o utils.o map.o player.o bomb.o bonus.o keyboard.o hud.o online.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

map.exe: map.o utils.o
	$(CC) $(CFLAGS)	$^ -o $@ $(LDFLAGS)	

*.o: *.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -f *.o

realclean: clean
	rm -f *.exe
