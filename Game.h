#include <ncurses.h>
#include <time.h>
#include "Screen.h"

#pragma once
class Game //GameManager의 역할
{
public:
	Screen screen; //Screen 객체
	bool gameOver; //게임오버 여부
	bool stageClear; // 스테이지 클리어 여부
	char second; //초
	int mapNumber; //맵 번호

	//미션 요소들 (인덱스: n번 스테이지)
	int missionLength[4] = {6, 8, 9, 9};
	int missionPlus[4] = {2, 3, 4, 6};
	int missionMinus[4] = {1, 2, 3, 4};
	int missionGate[4] = {1, 2, 3, 5};

	Game(int height, int width): second(0)
	{
		screen = Screen(height, width);
		screen.init(); //각 윈도우 clear, 출력
		gameOver = false;
		stageClear = false;
		mapNumber = 0; //시작 시 맵 번호 = 0
		toNextStage(); //시작 맵 출력
	}

	void toNextStage() //다음 스테이지로
	{
		screen.loadMap(mapNumber);
		makeMission(mapNumber++);
	}

	void makeMission(int mapNumber) //미션 생성
	{
		screen.addWithPosAndWin(3, 6, missionLength[mapNumber] + '0', screen.missionWin);
		screen.addWithPosAndWin(4, 6, missionPlus[mapNumber] + '0', screen.missionWin);
		screen.addWithPosAndWin(5, 6, missionMinus[mapNumber] + '0', screen.missionWin);
		screen.addWithPosAndWin(6, 6, missionGate[mapNumber] + '0', screen.missionWin);
	}

	void updateScreen() //화면 출력
	{
		screen.refresh();
	}

	chtype getInput() //입력 받기
	{
		chtype input = screen.getInput();
		return input;
	}

	void showSecond() //시간 표시
	{
		mvwaddch(screen.mainWin, 2, 10, second + '0');
		second++;
		updateScreen();
	}

	void addWithPos(int y, int x, chtype ch) //좌표 지정 후 문자 출력
	{
		screen.addWithPos(y, x, ch);
	}

	chtype getCharAt(int y, int x) //해당 좌표의 현재 문자 읽기
	{
		return screen.getCharAt(y, x);
	}
};