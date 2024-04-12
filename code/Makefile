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

all: moving.exe
	./moving.exe

moving.exe: moving.o utils.o
	$(CC) $^ -o $@ 

%.o: %.c
	$(CC) $(CFLAGS)	$^ -o $@ $(LDFLAGS)	

clean: 
	rm *.o
