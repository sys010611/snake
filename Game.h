#include <ncurses.h>
#include <time.h>
#include "Screen.h"

#pragma once
class Game //GameManager�� ����
{
public:
	Screen screen; //Screen ��ü
	bool gameOver; //���ӿ��� ����
	bool stageClear; // �������� Ŭ���� ����
	char second; //��
	int mapNumber; //�� ��ȣ

	//�̼� ��ҵ� (�ε���: n�� ��������)
	int missionLength[4] = {6, 8, 9, 9};
	int missionPlus[4] = {2, 3, 4, 6};
	int missionMinus[4] = {1, 2, 3, 4};
	int missionGate[4] = {1, 2, 3, 5};

	Game(int height, int width): second(0)
	{
		screen = Screen(height, width);
		screen.init(); //�� ������ clear, ���
		gameOver = false;
		stageClear = false;
		mapNumber = 0; //���� �� �� ��ȣ = 0
		toNextStage(); //���� �� ���
	}

	void toNextStage() //���� ����������
	{
		screen.loadMap(mapNumber);
		makeMission(mapNumber++);
	}

	void makeMission(int mapNumber) //�̼� ����
	{
		screen.addWithPosAndWin(3, 6, missionLength[mapNumber] + '0', screen.missionWin);
		screen.addWithPosAndWin(4, 6, missionPlus[mapNumber] + '0', screen.missionWin);
		screen.addWithPosAndWin(5, 6, missionMinus[mapNumber] + '0', screen.missionWin);
		screen.addWithPosAndWin(6, 6, missionGate[mapNumber] + '0', screen.missionWin);
	}

	void updateScreen() //ȭ�� ���
	{
		screen.refresh();
	}

	chtype getInput() //�Է� �ޱ�
	{
		chtype input = screen.getInput();
		return input;
	}

	void showSecond() //�ð� ǥ��
	{
		mvwaddch(screen.mainWin, 2, 10, second + '0');
		second++;
		updateScreen();
	}

	void addWithPos(int y, int x, chtype ch) //��ǥ ���� �� ���� ���
	{
		screen.addWithPos(y, x, ch);
	}

	chtype getCharAt(int y, int x) //�ش� ��ǥ�� ���� ���� �б�
	{
		return screen.getCharAt(y, x);
	}
};