UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)
CFLAGS = -I/opt/homebrew/include/SDL2 -D_THREAD_SAFE -Wall -g
LDFLAGS = -L/opt/homebrew/lib -lSDL2 -lSDL2_image
endif

ifeq ($(UNAME), Linux)
CFLAGS= -Wall -Werror
LDFLAGS= -lSDL2 -lSDL2_image -lSDL2_net
endif

CC = gcc

make: server_chat_simple.c client_chat_simple.c
	$(CC) $(CFLAGS) server_chat_simple.c -o server_chat_simple $(LDFLAGS)
	$(CC) $(CFLAGS) client_chat_simple.c -o client_chat_simple $(LDFLAGS)