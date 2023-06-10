#include <ncurses.h>
#include <iostream>
#include "Screen.h"
#include "Game.h"
#include "Item.h"
#include "Gate.h"


#define ROW_SIZE 30
#define COL_SIZE 90 // �׵θ� ũ��

int main()
{
	initscr();
	noecho();
	refresh();

	curs_set(0); //Ŀ�� �Ⱥ��̰�

	Game game = Game(ROW_SIZE, COL_SIZE); //���� ������Ʈ ����
	Item item = Item(game);
	Gate gate = Gate(game);

	game.updateScreen();

	int i = 0;
	while (true)
	{
		i++;
		if (i == 3)
		{
			item.Generate_item();
		}

		if (i == 6)
		{
			item.Delete_item();
			i = 0;
		}

		if (item.Gcnt == 3)
			item.cnt_init();

		game.showSecond(); //�ð���� ǥ�� (a,b,c,d...)

		gate.generateGate();

		chtype input = game.getInput();

		if (input == 'p') //���ӿ��� Ȱ��ȭ
			game.gameOver = true;
		if (input == 'n') //�������� Ŭ���� Ȱ��ȭ
			game.stageClear = true;

		if (game.gameOver) //���ӿ��� -> ,endwin()
			break;

		if (game.stageClear) //���� ����������
		{
			game.stageClear = false;
			game.toNextStage();
		}
	}

	endwin();

	return 0;
}