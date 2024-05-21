# IN104 Project: Bomberman

Implementation of the classic game Bomberman in C using SDL2.

This project is part of the course IN104 teached at ENSTA Paris

## Dependencies
Currently, only Linux and Mac Os are supported.

This game uses SDL2, SDL_net, SDL_image and SDL_ttf. You need to install those libraries on your machine in order to play the game.
For general explications you can follow [the SDL documentation](https://wiki.libsdl.org/SDL2/Installation).
For systems using apt you can run the following command:

```sudo apt-get install libsdl2-2.0-0 libsdl2-image-2.0-0 libsdl2-image-dev libsdl2-net-2.0-0 libsdl2-net-dev libsdl2-ttf-2.0-0 libsdl2-ttf-dev```


## Running the game

Clone the repository, go into the src subfolder and run ```make``` to launch the game.

If you have already compiled once, you can run ./main.exe for faster execution.

You can select which map you want to play one by running ```./main.exe ../maps/FILE``

## Tutorial

Once the game is launched you can chosse between local multiplayer and online multiplayer. The latter is not currently fully developed : bombs and explosions are 
not shown on the client side.

### Local multiplayer

The goal is to kill your opponent !

Player 1 is on the top-left corner and Player 2 is on the opposite corner (bottom-right). 
Player 1 (Player 2 respectively) moves with ZQSD (directional arrows) and places bombs with SPACE (RIGHT SHIFT)

At the beginning each player can place only one bomb at a time, moves at base speed and has an explosion radius of one tile. Explode soft walls to get powerups that will
give you advantages over your opponent !


## Authors
Arthur Foucard

Théo Atakpama

Supervised by Damien Bouchabou and Sao Mai Nguyen


