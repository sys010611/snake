all: main

main: main.cpp Screen.h Game.h Item.h Gate.h Snake.h Score.cpp
	g++ main.cpp -lncurses -o main