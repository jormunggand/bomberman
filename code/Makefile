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

%.o: %.c utils.c
	$(CC) $(CFLAGS)	$^ -o $@ $(LDFLAGS)	

clean: 
	rm *.o
