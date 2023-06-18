#include <ncurses.h>
#include <iostream>
#include <fstream>
#include <string>
#define MAP_WIDTH 40
#pragma once

class Screen
{
public:
	WINDOW* mainWin;
	WINDOW* scoreBoardWin;
	WINDOW* missionWin;

	char data[100][100]; //맵을 담는 2차원 배열

	Screen()
	{
		makeWindow(0, 0);
	}

	Screen(int height, int width)
	{
		makeWindow(height, width);
	}

	void init()
	{
		clear();
		makeInterface();
		refresh();
	}

	void makeInterface()
	{
		mvwprintw(scoreBoardWin, 1, 7, "Score Board");
		mvwprintw(scoreBoardWin, 3, 2, "B: ");
		mvwprintw(scoreBoardWin, 4, 2, "+: ");
		mvwprintw(scoreBoardWin, 5, 2, "-: ");
		mvwprintw(scoreBoardWin, 6, 2, "G: ");
		mvwprintw(scoreBoardWin, 7, 2, "Time: ");

		mvwprintw(missionWin, 1, 8, "Mission: ");
		mvwprintw(missionWin, 3, 2, "B: ");
		mvwprintw(missionWin, 4, 2, "+: ");
		mvwprintw(missionWin, 5, 2, "-: ");
		mvwprintw(missionWin, 6, 2, "G: ");
	}

	void drawBorder() //테두리 그리기
	{
		box(mainWin,0,0);
		box(scoreBoardWin,0,0);
		box(missionWin,0,0);
	}

	void addWithPos(int y, int x, chtype ch) //위치 지정 후 문자 추가
	{
		mvwaddch(mainWin, y, x, ch);
	}

	void addWithPosAndWin(int y, int x, chtype ch, WINDOW* win)
	{
		mvwaddch(win, y, x, ch);
	}

	chtype getInput()
	{
		return wgetch(mainWin);
	}

	chtype getCharAt(int y, int x)
	{
		return mvwinch(mainWin, y, x);
	}

	void clear() 
	{
		wclear(mainWin);
		wclear(scoreBoardWin);
		wclear(missionWin);
		drawBorder();
	}

	void refresh() //터미널 상에서 윈도우들 출력
	{
		wrefresh(mainWin);
		wrefresh(scoreBoardWin);
		wrefresh(missionWin);
	}

	void makeWindow(int height, int width) //윈도우 형성
	{
		int xMax, yMax;
		getmaxyx(stdscr, yMax, xMax);

		mainWin = newwin(height, width, yMax/2-height/2, xMax/2-width/2);
		scoreBoardWin = newwin(height/3, width/4, yMax/2-height/2.5, xMax/2 + width/8);
		missionWin = newwin(height/3, width/4, yMax/2, xMax/2+width/8);

		wtimeout(mainWin, 200); //0.2초마다 refresh
	}

	void loadMap(int mapNumber) // 맵 불러오기 (인자: 맵 번호)
	{
		std::ifstream mapFile;

		switch (mapNumber)
		{
			case 0:
				mapFile.open("map1.txt");
				break;
			case 1:
				mapFile.open("map2.txt");
				break;
			case 2:
				mapFile.open("map3.txt");
				break;
			case 3:
				mapFile.open("map4.txt");
				break;
			case 4:
				mapFile.open("GAME_CLEAR.txt");
				break;
			case 5:
				mapFile.open("STAGE_CLEAR.txt");
				break;
			case -1:
				mapFile.open("GAME_OVER.txt");
				break;
			default:
				return;				
		}

		int mapHeight = 0;

		while (!mapFile.eof())
		{
			char temp[120];
			mapFile.getline(temp, 120);

			for (int i = 0; i < MAP_WIDTH; i++)
			{
				data[mapHeight][i] = temp[i];
			}
			mapHeight++;
		}

		for (int i = 0; i < mapHeight; i++)
		{
			for (int j = 0; j < MAP_WIDTH; j++)
			{
				mvwaddch(mainWin, i+5,j+4,data[i][j]);
			}
		}
	}
};