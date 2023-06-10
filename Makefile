all: main

main: main.cpp Screen.h Game.h Item.h Gate.h
	g++ main.cpp -lncurses -o main