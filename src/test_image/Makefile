CC = gcc
CFLAGS = -I/opt/homebrew/include/SDL2 -D_THREAD_SAFE -Wall
LDFLAGS = -L/opt/homebrew/lib -lSDL2 -lSDL2_image 

all: load_image
	./load_image

load_image: load_image.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)	