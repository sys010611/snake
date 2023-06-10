#include <ncurses.h>
#include <iostream>
#include "Screen.h"
#include "Game.h"
#include "Item.h"
#include "Gate.h"


#define ROW_SIZE 30
#define COL_SIZE 90 // 테두리 크기

int main()
{
	initscr();
	noecho();
	refresh();

	curs_set(0); //커서 안보이게

	Game game = Game(ROW_SIZE, COL_SIZE); //게임 오브젝트 생성
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

		game.showSecond(); //시간경과 표시 (a,b,c,d...)

		gate.generateGate();

		chtype input = game.getInput();

		if (input == 'p') //게임오버 활성화
			game.gameOver = true;
		if (input == 'n') //스테이지 클리어 활성화
			game.stageClear = true;

		if (game.gameOver) //게임오버 -> ,endwin()
			break;

		if (game.stageClear) //다음 스테이지로
		{
			game.stageClear = false;
			game.toNextStage();
		}
	}

	endwin();

	return 0;
}